#ifndef TIME_H
#define TIME_H

// user setting..

#define TIM2_IQR_FUNC 
#define TIM3_IQR_FUNC
#define TIM4_IQR_FUNC 

// user setting--

//Tout= ((arr+1)*(psc+1))/Tclk
void time2_init(unsigned int arr,unsigned int psc);
void time3_init(unsigned int arr,unsigned int psc);
void time4_init(unsigned int arr,unsigned int psc);

#endif
