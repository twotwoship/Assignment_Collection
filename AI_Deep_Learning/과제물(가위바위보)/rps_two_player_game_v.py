import os
import time
import cv2
import numpy as np
import pycuda.autoinit  # pylint: disable=unused-import

from collections import Counter
from PIL import Image, ImageDraw, ImageFont
from trt_module import TRTInferenceEngine

ENGINE_PATH = 'rps_yolo11n_3.engine'

ansToText = {0: 'scissors', 1: 'rock', 2: 'paper'}
colorList = [(255, 0, 0), (0, 255, 0), (0, 0, 255)]

IMG_SIZE = 320
NUM_CLASSES = len(ansToText)
CONF_TH = 0.5
IOU_TH = 0.45

trt_engine = TRTInferenceEngine(ENGINE_PATH)
input_tensor = trt_engine.tensors[trt_engine.input_name]
output_tensor = trt_engine.tensors[trt_engine.output_name]
output_shape = tuple(output_tensor['shape'])

def letterbox(img, new_shape=(320, 320), color=(114, 114, 114)):
    h, w = img.shape[:2]
    nh, nw = new_shape
    r = min(nw / w, nh / h)

    new_w, new_h = int(w * r), int(h * r)
    resized = cv2.resize(img, (new_w, new_h))

    pad_w = nw - new_w
    pad_h = nh - new_h
    pad_x = pad_w // 2
    pad_y = pad_h // 2
    right = pad_w - pad_x
    bottom = pad_h - pad_y

    padded = cv2.copyMakeBorder(
        resized,
        pad_y, bottom,
        pad_x, right,
        cv2.BORDER_CONSTANT,
        value=color
    )

    return padded, r, pad_x, pad_y


def nms(boxes, scores, iou_th):
    if len(boxes) == 0:
        return []

    x1 = boxes[:, 0]
    y1 = boxes[:, 1]
    x2 = boxes[:, 2]
    y2 = boxes[:, 3]
    areas = (x2 - x1) * (y2 - y1)
    order = scores.argsort()[::-1]

    keep = []
    while order.size > 0:
        i = order[0]
        keep.append(i)

        xx1 = np.maximum(x1[i], x1[order[1:]])
        yy1 = np.maximum(y1[i], y1[order[1:]])
        xx2 = np.minimum(x2[i], x2[order[1:]])
        yy2 = np.minimum(y2[i], y2[order[1:]])

        w = np.maximum(0.0, xx2 - xx1)
        h = np.maximum(0.0, yy2 - yy1)
        inter = w * h
        iou = inter / (areas[i] + areas[order[1:]] - inter + 1e-6)

        inds = np.where(iou <= iou_th)[0]
        order = order[inds + 1]
    return keep

def processGameImage(frame):
    H, W = frame.shape[:2]

    img_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    img_lb, r, pad_x, pad_y = letterbox(img_rgb, (IMG_SIZE, IMG_SIZE))
    img = img_lb.astype(np.float32) / 255.0
    img = img.transpose(2, 0, 1)
    img = np.ascontiguousarray(img)

    input_data = np.expand_dims(img, axis=0)
    raw_output = trt_engine.infer(input_data)

    raw = raw_output.reshape(output_shape)[0]
    detections = raw.T

    cx = detections[:, 0]
    cy = detections[:, 1]
    w = detections[:, 2]
    h = detections[:, 3]

    cls_scores = detections[:, 4:4 + NUM_CLASSES]

    class_ids = np.argmax(cls_scores, axis=1)
    scores = np.max(cls_scores, axis=1)

    keep_idx = np.where(scores >= CONF_TH)[0]

    if len(keep_idx) == 0:
        return None, None

    cx = cx[keep_idx]
    cy = cy[keep_idx]
    w = w[keep_idx]
    h = h[keep_idx]
    scores = scores[keep_idx]
    class_ids = class_ids[keep_idx]

    x1 = np.clip((cx - w / 2.0 - pad_x) / r, 0, W).astype(np.int32)
    y1 = np.clip((cy - h / 2.0 - pad_y) / r, 0, H).astype(np.int32)
    x2 = np.clip((cx + w / 2.0 - pad_x) / r, 0, W).astype(np.int32)
    y2 = np.clip((cy + h / 2.0 - pad_y) / r, 0, H).astype(np.int32)

    boxes = np.stack([x1, y1, x2, y2], axis=1)
    keep = nms(boxes, scores, IOU_TH)

    team_a = None
    team_b = None

    team_a_score = 0.0
    team_b_score = 0.0

    for i in keep:
        bx1, by1, bx2, by2 = boxes[i]

        cid = int(class_ids[i])
        score = float(scores[i])

        if cid not in ansToText:
            continue

        gesture = ansToText[cid]

        center_x = (bx1 + bx2) // 2
        if center_x < W // 2:
            team_name = "A"

            if score > team_a_score:
                team_a_score = score
                team_a = {
                    "gesture": gesture,
                    "score": score,
                    "box": (bx1, by1, bx2, by2)
                }
        else:
            team_name = "B"

            if score > team_b_score:
                team_b_score = score
                team_b = {
                    "gesture": gesture,
                    "score": score,
                    "box": (bx1, by1, bx2, by2)
                }

        color = colorList[cid % len(colorList)]
        cv2.rectangle(
            frame,
            (bx1, by1),
            (bx2, by2),
            color,
            2
        )

        text = f"{team_name} : {gesture} {score:.2f}"
        cv2.putText(
            frame,
            text,
            (bx1, max(25, by1 - 7)),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.65,
            color,
            2
        )
    return team_a, team_b

def judge_rps(a, b):
    if a == b:
        return "和", "和"
    win_cases = {
        ("rock", "scissors"),
        ("scissors", "paper"),
        ("paper", "rock")
    }
    if (a, b) in win_cases:
        return "勝", "敗"
    return "敗", "勝"

def majority_vote(history):
    if not history:
        return None

    counter = Counter(history)
    return counter.most_common(1)[0][0]

CJK_FONT_CANDIDATES = [
    "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
    "/usr/share/fonts/opentype/noto/NotoSansCJK-Bold.ttc",
    "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
]

_cjk_font_cache = {}

def get_cjk_font(size):
    if size in _cjk_font_cache:
        return _cjk_font_cache[size]
    for font_path in CJK_FONT_CANDIDATES:
        if os.path.exists(font_path):
            font = ImageFont.truetype(font_path, size)
            _cjk_font_cache[size] = font
            return font
    raise RuntimeError(
        " 'fonts-noto-cjk' 폰트설치필요"
    )

def put_unicode_centered(img, text, center_x, center_y, font_size, color):
    pil_img = Image.fromarray(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    draw = ImageDraw.Draw(pil_img)
    font = get_cjk_font(font_size)

    bbox = draw.textbbox((0, 0), text, font=font)
    text_w = bbox[2] - bbox[0]
    text_h = bbox[3] - bbox[1]

    x = center_x - text_w // 2
    y = center_y - text_h // 2 - bbox[1]

    b, g, r = color
    draw.text((x, y), text, font=font, fill=(r, g, b))

    img[:] = cv2.cvtColor(np.array(pil_img), cv2.COLOR_RGB2BGR)


def draw_team_area(frame):
    H, W = frame.shape[:2]
    cv2.line(
        frame,
        (W // 2, 0),
        (W // 2, H),
        (255, 255, 255),
        2
    )
    cv2.rectangle(
        frame,
        (0, 0),
        (W, 45),
        (255, 255, 255),
        -1
    )

    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.9
    thickness = 2
    text_color = (0, 0, 0)

    a_text = "A TEAM"
    a_size = cv2.getTextSize(
        a_text,
        font,
        font_scale,
        thickness
    )[0]

    a_center_x = W // 4
    a_x = a_center_x - a_size[0] // 2

    cv2.putText(
        frame,
        a_text,
        (a_x, 32),
        font,
        font_scale,
        text_color,
        thickness
    )

    b_text = "B TEAM"
    b_size = cv2.getTextSize(
        b_text,
        font,
        font_scale,
        thickness
    )[0]

    b_center_x = (W * 3) // 4
    b_x = b_center_x - b_size[0] // 2

    cv2.putText(
        frame,
        b_text,
        (b_x, 32),
        font,
        font_scale,
        text_color,
        thickness
    )

GAME_WAIT = 0
GAME_COUNTDOWN = 1
GAME_RESULT = 2

game_state = GAME_WAIT

COUNTDOWN_TIME = 5.0

VOTE_WINDOW = 0.7

countdown_start = 0.0

a_gesture = None
b_gesture = None

a_result = None
b_result = None

a_history = []
b_history = []

current_team_a = None
current_team_b = None

_cached_panel = None
_cached_panel_key = None

CAMERA_WIDTH = 640
CAMERA_HEIGHT = 480

PANEL_HEIGHT = 180

DISPLAY_WIDTH = 640
DISPLAY_HEIGHT = CAMERA_HEIGHT + PANEL_HEIGHT

START_X1 = 220
START_Y1 = CAMERA_HEIGHT + 95
START_X2 = 420
START_Y2 = CAMERA_HEIGHT + 155

def mouse_callback(event, x, y, flags, param):
    global game_state
    global countdown_start

    global a_gesture
    global b_gesture

    global a_result
    global b_result

    global a_history
    global b_history

    if event != cv2.EVENT_LBUTTONDOWN:
        return

    if (
        START_X1 <= x <= START_X2
        and
        START_Y1 <= y <= START_Y2
    ):
        if game_state != GAME_COUNTDOWN:
            game_state = GAME_COUNTDOWN
            countdown_start = time.time()

            a_gesture = None
            b_gesture = None

            a_result = None
            b_result = None

            a_history.clear()
            b_history.clear()



def _render_game_panel():
    panel = np.full(
        (PANEL_HEIGHT, DISPLAY_WIDTH, 3),
        255,
        dtype=np.uint8
    )

    text_color = (0, 0, 0)

    if game_state != GAME_RESULT:

        if current_team_a is not None:
            a_text = (
                f"A : {current_team_a['gesture'].upper()} "
                f"{current_team_a['score']:.2f}"
            )
        else:
            a_text = "A : NO HAND"

        if current_team_b is not None:
            b_text = (
                f"B : {current_team_b['gesture'].upper()} "
                f"{current_team_b['score']:.2f}"
            )
        else:
            b_text = "B : NO HAND"

        cv2.putText(
            panel,
            a_text,
            (20, 55),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.85,
            text_color,
            2
        )

        cv2.putText(
            panel,
            b_text,
            (410, 55),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.85,
            text_color,
            2
        )

    else:
        a_text = (
            f"A : {a_gesture.upper()}"
            if a_gesture is not None
            else "A : NO HAND"
        )

        b_text = (
            f"B : {b_gesture.upper()}"
            if b_gesture is not None
            else "B : NO HAND"
        )

        cv2.putText(
            panel,
            a_text,
            (20, 50),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.9,
            text_color,
            2
        )

        cv2.putText(
            panel,
            b_text,
            (430, 50),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.9,
            text_color,
            2
        )
        if a_result in ("勝", "敗", "和"):
            a_color = (0, 0, 255) if a_result == "勝" else (0, 0, 0)
            put_unicode_centered(
                panel,
                a_result,
                110,
                108,
                58,
                a_color
            )
        else:
            cv2.putText(
                panel,
                a_result,
                (45, 120),
                cv2.FONT_HERSHEY_SIMPLEX,
                1.0,
                text_color,
                2
            )
        if b_result in ("勝", "敗", "和"):
            b_color = (0, 0, 255) if b_result == "勝" else (0, 0, 0)
            put_unicode_centered(
                panel,
                b_result,
                530,
                108,
                58,
                b_color
            )
        else:
            cv2.putText(
                panel,
                b_result,
                (445, 120),
                cv2.FONT_HERSHEY_SIMPLEX,
                1.0,
                text_color,
                2
            )

    button_color = (
        (220, 220, 220)
        if game_state == GAME_COUNTDOWN
        else (205, 190, 180)
    )

    cv2.rectangle(
        panel,
        (220, 95),
        (420, 155),
        button_color,
        -1
    )

    cv2.rectangle(
        panel,
        (220, 95),
        (420, 155),
        (100, 100, 100),
        2
    )

    button_text = (
        "COUNTDOWN"
        if game_state == GAME_COUNTDOWN
        else "START"
    )

    text_size = cv2.getTextSize(
        button_text,
        cv2.FONT_HERSHEY_SIMPLEX,
        1.0,
        2
    )[0]

    text_x = 320 - text_size[0] // 2

    cv2.putText(
        panel,
        button_text,
        (text_x, 136),
        cv2.FONT_HERSHEY_SIMPLEX,
        1.0,
        text_color,
        2
    )

    return panel


def make_game_panel():
    global _cached_panel
    global _cached_panel_key

    if game_state == GAME_RESULT:
        panel_key = (
            game_state,
            a_gesture,
            b_gesture,
            a_result,
            b_result
        )
    else:
        if current_team_a is not None:
            a_live = (
                current_team_a["gesture"],
                round(float(current_team_a["score"]), 2)
            )
        else:
            a_live = None

        if current_team_b is not None:
            b_live = (
                current_team_b["gesture"],
                round(float(current_team_b["score"]), 2)
            )
        else:
            b_live = None

        panel_key = (
            game_state,
            a_live,
            b_live
        )

    if _cached_panel is None or panel_key != _cached_panel_key:
        _cached_panel = _render_game_panel()
        _cached_panel_key = panel_key

    return _cached_panel


cap = cv2.VideoCapture(0)

cap.set(
    cv2.CAP_PROP_FRAME_WIDTH,
    CAMERA_WIDTH
)
cap.set(
    cv2.CAP_PROP_FRAME_HEIGHT,
    CAMERA_HEIGHT
)
cap.set(
    cv2.CAP_PROP_BUFFERSIZE,
    1
)
cv2.namedWindow(
    'cam',
    cv2.WINDOW_NORMAL
)
cv2.resizeWindow(
    'cam',
    DISPLAY_WIDTH,
    DISPLAY_HEIGHT
)
cv2.setMouseCallback(
    'cam',
    mouse_callback
)
startTime = time.time()

perf_sum = {
    "camera": 0.0,
    "yolo": 0.0,
    "draw_game": 0.0,
    "ui": 0.0,
    "compose": 0.0,
    "display": 0.0,
    "total": 0.0,
}
perf_count = 0
perf_report_time = time.perf_counter()

while cap.isOpened():
    frame_t0 = time.perf_counter()
    ret, frame = cap.read()
    t_camera = time.perf_counter()
    if not ret:
        break

    current_team_a, current_team_b = processGameImage(frame)
    t_yolo = time.perf_counter()
    draw_team_area(frame)
    if game_state == GAME_COUNTDOWN:

        elapsed = time.time() - countdown_start
        if elapsed >= COUNTDOWN_TIME - VOTE_WINDOW:

            if current_team_a is not None:
                a_history.append(
                    current_team_a["gesture"]
                )

            if current_team_b is not None:
                b_history.append(
                    current_team_b["gesture"]
                )


        if elapsed < 1.0:
            count_text = "3"
        elif elapsed < 2.0:
            count_text = "2"
        elif elapsed < COUNTDOWN_TIME:
            count_text = "1"
        else:
            count_text = None

            a_gesture = majority_vote(
                a_history
            )

            b_gesture = majority_vote(
                b_history
            )
            if (
                a_gesture is not None
                and
                b_gesture is not None
            ):
                a_result, b_result = judge_rps(
                    a_gesture,
                    b_gesture
                )
            elif (
                a_gesture is None
                and
                b_gesture is not None
            ):
                a_result = "NO HAND"
                b_result = "NO RESULT"
            elif (
                a_gesture is not None
                and
                b_gesture is None
            ):
                a_result = "NO RESULT"
                b_result = "NO HAND"
            else:
                a_result = "NO HAND"
                b_result = "NO HAND"

            game_state = GAME_RESULT
        if count_text is not None:

            text_size = cv2.getTextSize(
                count_text,
                cv2.FONT_HERSHEY_SIMPLEX,
                5,
                8
            )[0]

            text_x = (
                frame.shape[1] // 2
                - text_size[0] // 2
            )

            text_y = (
                frame.shape[0] // 2
                + text_size[1] // 2
            )

            cv2.putText(
                frame,
                count_text,
                (text_x, text_y),
                cv2.FONT_HERSHEY_SIMPLEX,
                5,
                (0, 0, 255),
                8
            )
    curTime = time.time()

    dt = curTime - startTime

    if dt > 0:
        fps = 1.0 / dt
    else:
        fps = 0.0

    startTime = curTime

    cv2.putText(
        frame,
        f'FPS: {fps:.1f}',
        (20, 80),
        cv2.FONT_HERSHEY_PLAIN,
        2,
        (0, 255, 255),
        2
    )

    t_draw_game = time.perf_counter()
    panel = make_game_panel()
    t_ui = time.perf_counter()

    display = np.vstack(
        [
            frame,
            panel
        ]
    )
    t_compose = time.perf_counter()

    cv2.imshow(
        'cam',
        display
    )

    key = cv2.waitKey(1)
    t_display = time.perf_counter()

    perf_sum["camera"] += (t_camera - frame_t0) * 1000.0
    perf_sum["yolo"] += (t_yolo - t_camera) * 1000.0
    perf_sum["draw_game"] += (t_draw_game - t_yolo) * 1000.0
    perf_sum["ui"] += (t_ui - t_draw_game) * 1000.0
    perf_sum["compose"] += (t_compose - t_ui) * 1000.0
    perf_sum["display"] += (t_display - t_compose) * 1000.0
    perf_sum["total"] += (t_display - frame_t0) * 1000.0
    perf_count += 1

    now_perf = time.perf_counter()

    if key == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
