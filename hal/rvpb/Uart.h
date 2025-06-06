#ifndef HAL_RVPB_UART_H_
#define HAL_RVPB_UART_H_

typedef union UARTDR_t
{
    uint32_t all;
    struct {
        uint32_t DATA:8;    
        uint32_t FE:1;    
        uint32_t PE:1;     
        uint32_t BE:1;   
        uint32_t OE:1;     
        uint32_t reserved:20;
    } bits;
} UARTDR_t;

typedef union UARTRSR_t
{
    uint32_t all;
    struct {
        uint32_t FE:1;   
        uint32_t PE:1;  
        uint32_t BE:1;  
        uint32_t OE:1;   
        uint32_t reserved:28;
    } bits;
} UARTRSR_t;

typedef union UARTFR_t
{
    uint32_t all;
    struct {
        uint32_t CTS:1;
        uint32_t DSR:1;   
        uint32_t DCD:1;
        uint32_t BUSY:1;
        uint32_t RXFE:1;
        uint32_t TXFF:1;  
        uint32_t RXFF:1;
        uint32_t TXFE:1;   
        uint32_t RI:1;
        uint32_t reserved:23;
    } bits;
} UARTFR_t;

typedef union UARTILPR_t
{
    uint32_t all;
    struct {
        uint32_t ILPDVSR:8; 
        uint32_t reserved:24;
    } bits;
} UARTILPR_t;

typedef union UARTIBRD_t
{
    uint32_t all;
    struct {
        uint32_t BAUDDIVINT:16; 
        uint32_t reserved:16;
    } bits;
} UARTIBRD_t;

typedef union UARTFBRD_t
{
    uint32_t all;
    struct {
        uint32_t BAUDDIVFRAC:6;
        uint32_t reserved:26;
    } bits;
} UARTFBRD_t;

typedef union UARTLCR_H_t
{
    uint32_t all;
    struct {
        uint32_t BRK:1;    
        uint32_t PEN:1;    
        uint32_t EPS:1;    
        uint32_t STP2:1; 
        uint32_t FEN:1;   
        uint32_t WLEN:2;   
        uint32_t SPS:1;  
        uint32_t reserved:24;
    } bits;
} UARTLCR_H_t;

typedef union UARTCR_t
{
    uint32_t all;
    struct {
        uint32_t UARTEN:1;     
        uint32_t SIREN:1;      
        uint32_t SIRLP:1;      
        uint32_t Reserved1:4;  
        uint32_t LBE:1;
        uint32_t TXE:1;
        uint32_t RXE:1;
        uint32_t DTR:1;
        uint32_t RTS:1;
        uint32_t Out1:1;
        uint32_t Out2:1;
        uint32_t RTSEn:1;
        uint32_t CTSEn:1;
        uint32_t reserved2:16;
    } bits;
} UARTCR_t;

typedef union UARTIFLS_t
{
    uint32_t all;
    struct {
        uint32_t TXIFLSEL:3;
        uint32_t RXIFLSEL:3;
        uint32_t reserved:26;
    } bits;
} UARTIFLS_t;

typedef union UARTIMSC_t
{
    uint32_t all;
    struct {
        uint32_t RIMIM:1;   
        uint32_t CTSMIM:1;  
        uint32_t DCDMIM:1;  
        uint32_t DSRMIM:1;  
        uint32_t RXIM:1;    
        uint32_t TXIM:1;    
        uint32_t RTIM:1;    
        uint32_t FEIM:1;    
        uint32_t PEIM:1;    
        uint32_t BEIM:1;    
        uint32_t OEIM:1;    
        uint32_t reserved:21;
    } bits;
} UARTIMSC_t;

typedef union UARTRIS_t
{
    uint32_t all;
    struct {
        uint32_t RIRMIS:1;  
        uint32_t CTSRMIS:1; 
        uint32_t DCDRMIS:1; 
        uint32_t DSRRMIS:1; 
        uint32_t RXRIS:1;   
        uint32_t TXRIS:1;   
        uint32_t RTRIS:1;   
        uint32_t FERIS:1;   
        uint32_t PERIS:1;   
        uint32_t BERIS:1;   
        uint32_t OERIS:1;   
        uint32_t reserved:21;
    } bits;
} UARTRIS_t;

typedef union UARTMIS_t
{
    uint32_t all;
    struct {
        uint32_t RIMMIS:1; 
        uint32_t CTSMMIS:1; 
        uint32_t DCDMMIS:1;
        uint32_t DSRMMIS:1; 
        uint32_t RXMIS:1;   
        uint32_t TXMIS:1;   
        uint32_t RTMIS:1;   
        uint32_t FEMIS:1;   
        uint32_t PEMIS:1;   
        uint32_t BEMIS:1; 
        uint32_t OEMIS:1;  
        uint32_t reserved:21;
    } bits;
} UARTMIS_t;

typedef union UARTICR_t
{
    uint32_t all;
    struct {
        uint32_t RIMIC:1;  
        uint32_t CTSMIC:1; 
        uint32_t DCDMIC:1;  
        uint32_t DSRMIC:1; 
        uint32_t RXIC:1;   
        uint32_t TXIC:1; 
        uint32_t RTIC:1;   
        uint32_t FEIC:1;   
        uint32_t PEIC:1;    
        uint32_t BEIC:1;    
        uint32_t OEIC:1;  
        uint32_t reserved:21;
    } bits;
} UARTICR_t;

typedef union UARTDMACR_t
{
    uint32_t all;
    struct {
        uint32_t RXDMAE:1;  
        uint32_t TXDMAE:1;  
        uint32_t DMAONERR:1;
        uint32_t reserved:29;
    } bits;
} UARTDMACR_t;

typedef struct PL011_t
{
    UARTDR_t    uartdr;         //0x000
    UARTRSR_t   uartrsr;        //0x004
    uint32_t    reserved0[4];   //0x008-0x014
    UARTFR_t    uartfr;         //0x018
    uint32_t    reserved1;      //0x01C
    UARTILPR_t  uartilpr;       //0x020
    UARTIBRD_t  uartibrd;       //0x024
    UARTFBRD_t  uartfbrd;       //0x028
    UARTLCR_H_t uartlcr_h;      //0x02C
    UARTCR_t    uartcr;         //0x030
    UARTIFLS_t  uartifls;       //0x034
    UARTIMSC_t  uartimsc;       //0x038
    UARTRIS_t   uartris;        //0x03C
    UARTMIS_t   uartmis;        //0x040
    UARTICR_t   uarticr;        //0x044
    UARTDMACR_t uartdmacr;      //0x048
} PL011_t;

#define UART_BASE_ADDRESS0       0x10009000
#define UART_INTERRUPT0          44

#endif /* HAL_RVPB_UART_H_ */