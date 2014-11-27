; Mark Bowers - Oakland University
; HCS12 VGA - rev 1
; http://markbowers.org/home/hcs12-vga


; export symbols
            XDEF asm_main
            XDEF PLL_setup
            XDEF set_video_pointer
            XDEF video_draw
            XDEF video_enable
            XDEF ms_delay
            XDEF seg7dec
            ; we use export 'Entry' as symbol. This allows us to
            ; reference 'Entry' either in the linker .prm file
            ; or from C/C++ later on

; Include derivative-specific definitions 
		INCLUDE 'derivative.inc'  		
		    
; variable/data section
MY_EXTENDED_RAM: SECTION
; Insert here your data definition. For demonstration, temp_byte is used.


; ----------- VIDEO VARIABLES ----------



vid_pointer:      DS.W  1   ; pointer to beginning of video buffer
vid_blanks:       DS.B  1   ; blanking counter

; code section
MyCode:     SECTION
; this assembly routine is called by the C/C++ application






NumArray: DC.b   $99, $12, $EF, $23, $AB, $13, $66, $EF 
          
SEG7TBL:  DC.B    $40,$79,$24,$30
		      DC.B    $19,$12,$02,$78
		      DC.B    $00,$10,$08,$03
		      DC.B    $46,$21,$06,$0E          

; setup PLL = 50MHz
; PLLCLK = crystal * 2 * (SYNR+1)/(REFDV+1)
; 50MHz = 16MHz * 2 * (SYNR+1)/(REFDV+1)
; 50/32 = 25/16 => SYNR = 24, REFDV = 15
; So SYNR = 0x18, REFDV = 0x0F
  PLL_setup: 
             sei                        ; interrupts disabled
             bclr CLKSEL,%10000000      ; switch clock to OSCCLK
             bset PLLCTL,%01000000      ; turn on PLL
             movb #$18, SYNR            ; set clock multiplier
             movb #$0F, REFDV           ; set clock divider
  PLL_lock:  brclr CRGFLG,$08, PLL_lock ; wait until locked
             bset CLKSEL,%10000000      ; switch clock to OSCCLK
             bclr PEAR,$10              ; make ECLK visible on PE4 (pin 39)
             rts


 ; ---------- DELAY MACROS ----------

     ; waste 1 cycle
     nop1: macro   
              nop
              endm
              
     ; waste 2 cycles
     nop2: macro   
              nop
              nop
              endm
              
     ; waste 5 cycles
     nop5: macro   
              nop 
              nop 
              nop
              nop
              nop
              endm
              

 ; ---------- DELAY SUBROUTINES ------
  
    ; waste 10 cycles
     nop10:        ; cycles from JSR (4 cycles)
              nop  ;       burn time (1 cycle)  
              rts  ; return from sub (5 cycles)
  

    ; waste 100 cycles
    nop100:               
                          ; cycles from JSR      (4 cycles)    
              pshd        ; push D to the stack  (2 cycles)
              ldaa #21    ; load A               (1 cycle)
              
    loop100:  deca        ; A--                  (1 cycle)
              bne loop100 ; loop if not zero     (3 cycles)   <- BRANCH             
              nop2        ; burn time            (1+2 cycles) <- NO BRANCH
              ; LOOP TOTAL = 4*21 = (84 cycles)
              
              nop         ; burn time            (1 cycle)
              puld        ; pull D from stack    (3 cycles)
              rts         ; return from sub      (5 cycles)
              
              ; TOTAL =(4+2+1)+(84)+(1+3+5)=100 

; ---------- VIDEO MACROS ----------

   ; writes a pixel color to PORTA
   video_1:  macro
              inx           ; increment X    (1 cycle)
              nop2          ; waste time     (2 cycles)
              movb X, PORTA ; write to pins  (5 cycles)         
              endm
   
   video_9:  macro          ; 8x9 => 72 cycles
              video_1 
              video_1 
              video_1 
              video_1 
              video_1
              video_1 
              video_1 
              video_1 
              video_1
              endm
          
   video_10: macro         ; 8x10 => 80 cycles
              video_9 
              video_1
              endm


; video drawing routine
; PJ0 = HSYNC
; PJ1 = VSYNC
; PORTA = COLOR
video_draw:
           ; *****************************************************
           ; *    ATTENTION: This function is timing-critical    *
           ; *****************************************************

           tst vid_blanks        ; check if blank count is zero   (3 cycles)  
           beq start_active      ; go begin active video section 
            
           ; common                                (1 cycle) <- NOT BRANCHED
           ldaa vid_blanks       ; load A          (3 cycles)
           cmpa #31              ; VSYNC start?    (1 cycle)
           beq vs_low 
            
           ; (A/=31)           ;                   (1 cycle) <- NOT BRANCHED
           cmpa #29            ; VSYNC end?        (1 cycle)
           beq vs_high      
            
           ; else                                  (1 cycle) <- NOT BRANCHED
           bclr PTJ,%00000001      ; HS=0          (4 cycles)
           nop                     ; burn time     (1 cycle)                  
           bra hs_pulse 
           ; TOTAL FROM START=(3)+(1+3+1)+(1+1)+(1+4+1)=17
           
            ; (A=31) 
    vs_low: ; bring VSYNC low                      (3 cycles) <- BRANCHED   
           movb #%00000000,PTJ     ; VS=0, HS=0    (4 cycles)
           nop2                    ; burn time     (2 cycles)
           bra hs_pulse
           ; TOTAL FROM START=(3)+(1+3+1)+(3+4+2)=17

            ; (A=29)    
   vs_high: ; bring VSYNC high                     (3 cycles) <- BRANCHED
           movb #%00000010,PTJ     ; VS=1, HS=0    (4 cycles)
           bra hs_pulse
           ; TOTAL FROM START=(3)+(1+3+1)+(1+1)+(3+4)=17
 
  hs_pulse:  
            ; ----- BLANK HORIZONTAL SYNC PULSE = 128 cycles -----
                               ; delay from entry = (3 cycles)
            dec vid_blanks     ; dec blank counter  (4 cycles)
            nop2                       ; burn time  (2 cycles)
            nop                        ; burn time  (1 cycle)
            jsr nop100                 ; burn time  (100 cycles)
            jsr nop10                  ; burn time  (10 cycles)
            movb #$80,MCFLG ; clear interrupt flag  (4 cycles)
            bset PTJ,%00000001          ; raise HS  (4 cycles)
            rts ; return from subroutine

 start_active: 
            ; prepare for active video section       (3 cycles) <- BRANCHED
            sei           ; disable video interrupt  (1 cycle)
            ldx  vid_pointer    ; video_pointer      (3 cycles)
            clra                ; V repeat counter   (1 cycle)
            clrb                ; V rows counter     (1 cycle)
            nop                 ; burn time          (1 cycle)
            bclr PTJ,%00000001  ; lower HS for sync  (4 cycles)
            ; TOTAL FROM START=(3)+(3+1+3+1+1+1+4)=17    
            nop2
            nop2
 a_hs_pulse:
            ; ----- ACTIVE HORIZONTAL SYNC PULSE = 128 cycles -----
                               ; delay from entry = (4 cycles)
            jsr nop100                 ; burn time  (120 cycles)
            jsr nop10                  
            jsr nop10 
            bset PTJ,%00000001         ; raise HS   (4 cycles)
            
            ; ----- HORIZONTAL BACK PORCH = 16 cycles -----
            jsr nop10     ; burn time               (10 cycles)
            tfr X, Y      ; line start addr -> Y    (1 cycle)
            movb X, PORTA ; write first pixel       (5 cycles)
            
            ; ----- HORIZONTAL ACTIVE VIDEO = 640 cycles -----
            video_10  ;                     (80 cycles)
            video_10  ;                     (80 cycles)
            video_10  ;                     (80 cycles)
            video_10  ;                     (80 cycles)
            video_10  ;                     (80 cycles)
            video_10  ;                     (80 cycles)
            video_10  ;                     (80 cycles)
            video_9   ;                     (72 cycles)
            inx       ; increment for next  (1 cycle)
            nop2      ; burn time           (2 cycle)
            nop2      ; burn time           (2 cycle)
            clr PORTA ; blacken video       (3 cycles)

            ; ----- HORIZONTAL FRONT PORCH = 16 cycles -----
            ; if B=7, A=59 => EXIT
            ; elsif B=7    => A++, B=0
            ; else         => B++, X=X-80

            cmpb #7        ; compare B with 7      (1 cycle)
            beq done_reps  ; branch if B = 7     
            
            ; (B!=7)                      
            ; more repeats to go                   (1 cycle) <- NOT BRANCHED
            incb                ; inc repeat count (1 cycle)          
            tfr Y, X            ; start line again (1 cycle)
            nop2                ; burn time        (2 cycles)
            nop2                ; burn time        (2 cycles) 
            bclr PTJ,%00000001  ; lower HS         (4 cycles)       
            lbra a_hs_pulse     ; go repeat row    (4 cycles)
            ; TOTAL=(1)+(1+1+1+2+2+4+4)=16
            
 done_reps: ; (B=7)                                (3 cycles) <- BRANCHED            
            clrb          ; clear B                (1 cycle)
            cmpa #59      ; compare A with 59      (1 cycle)
            beq done_rows ; branch if A = 59
            
            ; (B=7) && (A!=0)
            ; move to next line                    (1 cycle) <- NOT BRANCHED
            inca                ; inc line count   (1 cycle)
            bclr PTJ,%00000001  ; lower HS         (4 cycles)  
            lbra a_hs_pulse     ; go new line      (4 cycles)
            ; TOTAL=(1)+(3+1+1)+(1+1+4+4)=16
            
            ; (B=7) && (A=59)                      (3 cycle) <- BRANCHED
 done_rows: nop2                 ; burn time       (2 cycles)
            nop1                 ; burn time       (1 cycles)     
            bclr PTJ,%00000001   ; lower HS        (4 cycles)
            ; TOTAL=(1)+(3+1+1)+(3+2+1+4)=16

            ; ----- NEXT HORIZONTAL SYNC PULSE = 128 cycles -----
            jsr nop100           ; burn time              (100 cycles)
            jsr nop10            ; burn time              (10 cycles)
            nop5                 ; burn time              (5 cycles)
            movb #40, vid_blanks ; 40 blank lines         (4 cycles)           
            movb #$80,MCFLG      ; clear interrupt flag   (4 cycles)
            cli                  ; re-enable interrupts   (1 cycle)
            bset PTJ,%00000001   ; raise HS               (4 cycles)

            ; end active video!
            rts                ; return from subroutine   (5 cycles)
                               ; return from interrupt    (8 cycles)
            


             
; setup modulus down-counter interrupt, to fire at 31.25kHz
; MCCTL = 11000100 (no prescale)
; MCCNT = 25MHz/31.25kHz = 800
video_enable:
              movb #$CA,HPRIO       ; elevate MDC to highest interrupt priority    
              movb #%11000100,MCCTL ; enable MDC, no prescale
              movw #800,MCCNT       ; set count value
              bset PTJ,%00000011    ; set PJ0 and PJ1 high
              clr  PORTA            ; clear PORTA             
              bset DDRJ,%00000011   ; set PJ0 and PJ1 output
              movb #$FF,DDRA        ; set PORTA output
              cli                   ; enable interrupts            
              rts 

; set video pointer
set_video_pointer:
            std vid_pointer
            ; movb #00, vid_blanks
            rts



                       
;       7-segment decoder
;       input: B = index into SEG7TBL
seg7dec:
          movb    #$FF, DDRH ; set outputs
          ldy     #SEG7TBL
          aby               ;y -> 7-seg code
          ldaa    0,y
          staa    PTH
          rts  
        
;       ms_delay                              
;       input: D = no. of milliseconds to delay
;       clock = 24 MHz
ms_delay:
          pshx
          pshy
          tfr     D,Y
md1:      ldx     #5999   ; N = (24000 - 5)/4
md2:      dex             ; 1 ccycle
          bne     md2     ; 3 ccycle
          dey             
          bne     md1     ; Y ms
          puly
          pulx
          rts
          

asm_main:
            


