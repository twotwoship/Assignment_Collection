used function 

Ignore the comments.





void \_non\_delay\_left\_make\_trigger\_processing(void){ // 딜레이 함수 쓰지 않고 초음파 쏘기 좌 통합

&#x09;// 어떻게 돌아가야하냐면 하이상태를 15us 유지하고 로우로 내려야됨.

&#x09;// 내리는건 타이머 인터럽트에서 해버리자.

&#x09;if(high\_left\_pulse == 0 \&\& low\_left\_pulse == 0 \&\& \_non\_delay\_left\_trigger\_wait == 0){	

&#x09;	\_non\_delay\_left\_make\_trigger\_high();		// 쏘고나서 15us 만큼 기다려야됨

&#x09;	\_non\_delay\_left\_trigger\_wait = \_us\_count;

&#x09;}

&#x09;if(\_non\_delay\_left\_trigger\_wait != 0){ // 하이로 쏜상태임.

&#x09;	if ((\_us\_count - \_non\_delay\_left\_trigger\_wait) >= 1)

&#x09;	{

&#x09;		\_non\_delay\_left\_make\_trigger\_low();

&#x09;	}

&#x09;}

&#x09;if(high\_left\_pulse == 1 \&\& low\_left\_pulse == 1){ // 이거는 이제 에코가 다 들어오고나면 000으로 다 내려서 다시 하이를 쏠수있게 준비

&#x09;	high\_left\_pulse = 0;

&#x09;	low\_left\_pulse = 0;

&#x09;	\_non\_delay\_left\_trigger\_wait = 0;

&#x09;}

}



ISR(INT4\_vect){	// echo핀에서 나온 게 들어간다 보드로

&#x09;// 타이머 2번을 쓸꺼임 타이머 2번은 분주를 하지 않음.

&#x09;//tcnt2 노분주로 타이머2인터럽트가 들어올때마다 1개씩 커짐.

&#x09;if(ECHO\_LAFT\_PORT \& ( 1 << ECHO\_LAFT\_PIN)){	// 1. high edge

&#x09;	left\_count = \_us\_count;		// 딱 상승이 발생햇어

&#x09;	high\_left\_pulse = 1;

&#x09;}else{	// 2. low edge

&#x09;	low\_left\_pulse = 1;

&#x09;	\_non\_delay\_left\_trigger\_wait = 0;

&#x09;}

}



ISR(TIMER2\_OVF\_vect){		// 16us timer 오버플로발생해서 저시간마다 들어옴.

&#x09;TCNT2 = 0;				// 0.016/256 카운터 ㅋㅋ 1틱당 0.625us

&#x09;\_us\_count++;			//  16us timer 마다 커지는거임.

&#x09;if(\_us\_count> \_US\_COUNT\_RESET){

&#x09;	\_us\_count= 1;		// 트리거 체크때문에 0으로 만들면 안됨.

&#x09;}

}







&#x09;while(1){	

&#x09;	\_non\_delay\_left\_make\_trigger\_processing();

&#x09;	}



