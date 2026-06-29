/*
 * Speaker.h
 *
 * Created: 2026-06-25 오후 3:00:21
 *  Author: kccistc
 */ 


#ifndef SPEAKER_H_
#define SPEAKER_H_

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DO_01   1911
#define DO_01_H 1817
#define RE_01   1703
#define RE_01_H 1607
#define MI_01   1517
#define FA_01   1432
#define FA_01_H 1352
#define SO_01   1276
#define SO_01_H 1199
#define LA_01   1136
#define LA_01_H 1073
#define TI_01   1012
#define DO_02   956
#define DO_02_H 909
#define RE_02   851
#define RE_02_H 804
#define MI_02   758
#define MI_02_H 758
#define FA_02   716
#define FA_02_H 676
#define SO_02   638
#define SO_02_H 602
#define LA_02   568
#define LA_02_H 536
#define TI_02   506
#define DO_03   478
#define DO_03_H 450
#define RE_03	425
#define RE_03_H 401
#define MI_03	378


#define F_CLK       16000000L //클럭
#define F_SCALER	8 //프리스케일러
#define BEAT_1_32	42
#define BEAT_1_16	86
#define BEAT_1_8	170
#define BEAT_1_4_3	113
#define BEAT_1_4	341
#define BEAT_1_2	682
#define BEAT_1		1364
/*
#define DO_01   (F_CLK/(2*16.3516*F_SCALER)-1)
#define DO_01s  (F_CLK/(2*17.3239*F_SCALER)-1)
#define RE_01   (F_CLK/(2*18.3541*F_SCALER)-1)
#define RE_01s  (F_CLK/(2*19.4454*F_SCALER)-1)
#define MI_01   (F_CLK/(2*20.6017*F_SCALER)-1)
#define FA_01   (F_CLK/(2*21.8268*F_SCALER)-1)
#define FA_01s  (F_CLK/(2*23.1247*F_SCALER)-1)
#define SO_01   (F_CLK/(2*24.4997*F_SCALER)-1)
#define SO_01s  (F_CLK/(2*25.9565*F_SCALER)-1)
#define LA_01   (F_CLK/(2*27.5*F_SCALER)-1)
#define LA_01s  (F_CLK/(2*29.1352*F_SCALER)-1)
#define SI_01   (F_CLK/(2*30.8677*F_SCALER)-1)
#define DO_02   (F_CLK/(2*32.7032*F_SCALER)-1)
#define DO_02s  (F_CLK/(2*34.6478*F_SCALER)-1)
#define RE_02   (F_CLK/(2*36.7081*F_SCALER)-1)
#define RE_02s  (F_CLK/(2*38.8909*F_SCALER)-1)
#define MI_02   (F_CLK/(2*41.2034*F_SCALER)-1)
#define FA_02   (F_CLK/(2*43.6535*F_SCALER)-1)
#define FA_02s  (F_CLK/(2*46.2493*F_SCALER)-1)
#define SO_02   (F_CLK/(2*48.9994*F_SCALER)-1)
#define SO_02s  (F_CLK/(2*51.913*F_SCALER)-1)
#define LA_02   (F_CLK/(2*55*F_SCALER)-1)
#define LA_02s  (F_CLK/(2*58.2705*F_SCALER)-1)
#define SI_02   (F_CLK/(2*61.7354*F_SCALER)-1)
#define DO_03   (F_CLK/(2*65.4064*F_SCALER)-1)
#define DO_03s  (F_CLK/(2*69.2957*F_SCALER)-1)
#define RE_03   (F_CLK/(2*73.4162*F_SCALER)-1)
#define RE_03s  (F_CLK/(2*77.7817*F_SCALER)-1)
#define MI_03   (F_CLK/(2*82.4069*F_SCALER)-1)
#define FA_03   (F_CLK/(2*87.3071*F_SCALER)-1)
#define FA_03s  (F_CLK/(2*92.4986*F_SCALER)-1)
#define SO_03   (F_CLK/(2*97.9989*F_SCALER)-1)
#define SO_03s  (F_CLK/(2*103.8262*F_SCALER)-1)
#define LA_03   (F_CLK/(2*110*F_SCALER)-1)
#define LA_03s  (F_CLK/(2*116.5409*F_SCALER)-1)
#define SI_03   (F_CLK/(2*123.4708*F_SCALER)-1)
#define DO_04   (F_CLK/(2*130.8128*F_SCALER)-1)
#define DO_04s  (F_CLK/(2*138.5913*F_SCALER)-1)
#define RE_04   (F_CLK/(2*146.8324*F_SCALER)-1)
#define RE_04s  (F_CLK/(2*155.5635*F_SCALER)-1)
#define MI_04   (F_CLK/(2*164.8138*F_SCALER)-1)
#define MI_04s  (F_CLK/(2*174.6141*F_SCALER)-1)
#define FA_04   (F_CLK/(2*174.6141*F_SCALER)-1)
#define FA_04s  (F_CLK/(2*184.9972*F_SCALER)-1)
#define SO_04   (F_CLK/(2*195.9977*F_SCALER)-1)
#define SO_04s  (F_CLK/(2*207.6523*F_SCALER)-1)
#define LA_04   (F_CLK/(2*220*F_SCALER)-1)
#define LA_04s  (F_CLK/(2*233.0819*F_SCALER)-1)
#define SI_04   (F_CLK/(2*246.9417*F_SCALER)-1)
#define DO_05   (F_CLK/(2*261.6256*F_SCALER)-1)
#define DO_05s  (F_CLK/(2*277.1826*F_SCALER)-1)
#define RE_05   (F_CLK/(2*293.6648*F_SCALER)-1)
#define RE_05s  (F_CLK/(2*311.127*F_SCALER)-1)
#define MI_05   (F_CLK/(2*329.6276*F_SCALER)-1)
#define FA_05   (F_CLK/(2*349.2282*F_SCALER)-1)
#define FA_05s  (F_CLK/(2*369.9944*F_SCALER)-1)
#define SO_05   (F_CLK/(2*391.9954*F_SCALER)-1)
#define SO_05s  (F_CLK/(2*415.3047*F_SCALER)-1)
#define LA_05   (F_CLK/(2*440*F_SCALER)-1)
#define LA_05s  (F_CLK/(2*466.1638*F_SCALER)-1)
#define SI_05  (F_CLK/(2*493.8833*F_SCALER)-1)
#define DO_06   (F_CLK/(2*523.2511*F_SCALER)-1)
#define DO_06s  (F_CLK/(2*554.3653*F_SCALER)-1)
#define RE_06   (F_CLK/(2*587.3295*F_SCALER)-1)
#define RE_06s  (F_CLK/(2*622.254*F_SCALER)-1)
#define MI_06   (F_CLK/(2*659.2551*F_SCALER)-1)
#define FA_06   (F_CLK/(2*698.4565*F_SCALER)-1)
#define FA_06s  (F_CLK/(2*739.9888*F_SCALER)-1)
#define SO_06   (F_CLK/(2*783.9909*F_SCALER)-1)
#define SO_06s  (F_CLK/(2*830.6094*F_SCALER)-1)
#define LA_06   (F_CLK/(2*880*F_SCALER)-1)
#define LA_06s  (F_CLK/(2*932.3275*F_SCALER)-1)
#define SI_06   (F_CLK/(2*987.7666*F_SCALER)-1)
#define DO_07   (F_CLK/(2*1046.502*F_SCALER)-1)
#define DO_07s  (F_CLK/(2*1108.731*F_SCALER)-1)
#define RE_07   (F_CLK/(2*1174.659*F_SCALER)-1)
#define RE_07s  (F_CLK/(2*1244.508*F_SCALER)-1)
#define MI_07   (F_CLK/(2*1318.51*F_SCALER)-1)
#define FA_07   (F_CLK/(2*1396.913*F_SCALER)-1)
#define FA_07s  (F_CLK/(2*1479.978*F_SCALER)-1)
#define SO_07   (F_CLK/(2*1567.982*F_SCALER)-1)
#define SO_07s  (F_CLK/(2*1661.219*F_SCALER)-1)
#define LA_07   (F_CLK/(2*1760*F_SCALER)-1)
#define LA_07s  (F_CLK/(2*1864.655*F_SCALER)-1)
#define SI_07   (F_CLK/(2*1975.533*F_SCALER)-1)
*/

void Music_Player(int *tone, int *Beats);
void init_speaker(void);
void Beep(int repeat);
void Siren(int repeat);
void RRR(void);
void delay_ms(int ms);

#endif /* SPEAKER_H_ */