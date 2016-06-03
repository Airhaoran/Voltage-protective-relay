#include <stdio.h>
#include <project.h>
#include <math.h>

int Index;
uint32 buff[48] = {137, 100,37, -37,-100,-137,-137,-100,-37,37,100,137,137, 100,37, -37,-100,-137,-137,-100,-37,37,100,137,137,
    100,37, -37,-100,-137,-137,-100,-37,37,100,137,137, 100,37, -37,-100,-137,-137,-100,-37,37,100,137};

//uint32 filtered[100];
int coe[12] = {1, 1, 1, -1,-1,-1,-1,-1,-1,1,1,1};
//double myFloat = 0.23123;
int32 filteredData[100];
int32 ch1Mag[100];
CY_ISR_PROTO(MYIsrHandler);
void main()
{
    
    int i; 
    int c;
    int a;
    CyGlobalIntEnable;
    Clock_1_Start();
    //Clock_2_Start();
    Counter_1_Start();
    UART_1_Start();
    ADC_SAR_Seq_1_Start();
    printf("Project start.......\r\n");
    printf("Interrupt start.......\r\n");
    //isr_1_StartEx(MYIsrHandler);
    myFilter(coe, 12, buff, filteredData, 47);
    magCal(filteredData, 47, chiMag);
    for(i=0; i<=12;i++)
        {
            //printf("buff[%d] = %d\r\n",i, buff[i]);
            printf("%d\r\n", buff[i]);
        }
    printf("filtered:\r\n");
    for(c=11; c<=47;c++)
        {
            //printf("buff[%d] = %d\r\n",i, buff[i]);
            printf("%d\r\n",filteredData[c]);
        }
    printf("Maglitude:\r\n");
    for (int a = 14; i < 47; a++)
    {
      printf("mag[%d] = %d\r\n",a++ );
    }
    
    /*for(;;)
    {
       if(Index == 37){
        printf("Reach 32 samples in buffer\r\n");
        isr_1_Stop();
        myFilter(coe, 12, buff, filteredData, 12);
        for(i=0; i<=Index-1;i++)
        {
            //printf("buff[%d] = %d\r\n",i, buff[i]);
            printf("%d\r\n", buff[i]);
        }
        for(c=0; c<=Index-1;c++)
        {
            //printf("buff[%d] = %d\r\n",i, buff[i]);
            printf("%d\r\n",filtered[i]);
        }
        return 0;}
 
    }*/
    //return 0;
}

void myFilter (int32 *p_coeffs, int p_coeffs_n, int32 *p_in, int32 *output, int n)
 {
   int i, j, k;
   int32 tmp;
  
   for (k = 0; k <= n+1; k++) // position in output
   {
      tmp = 0;

      for (i = 0; i < p_coeffs_n; i++) // position in coefficients array
      {
         j = k - i; // position in input

         if (j >= 0) // bounds check for input buffer
         { 
           printf("buff[%d]= %d, coe[%d] = %d\r\n",j,p_in[j],i,p_coeffs[i]);
           tmp += p_coeffs [i] * p_in [j];
         }
       } 
      printf("temp =%d\r\n", tmp);
     
      output[k] = tmp/6; 
      printf("output[%d] =%d\r\n", k,output[k] );
   }

}

void magCal(int32 *filteredData, int n, int32 *magCh1)
{
  //int i, j;
  //int32 temp;
  if (n >2)
  {
    magCh1[n] = sqr(filteredData[n-3]*filteredData[n-3]+filteredData[n]*filteredData[n]);
  }

}

int _write(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        UART_1_UartPutChar(*ptr++);
    }
    return len;
}




CY_ISR(MYIsrHandler)
{
    uint16 ADCResult;
    buff[Index] = ADC_SAR_Seq_1_GetResult16(0);
    
    Index++;
    
}