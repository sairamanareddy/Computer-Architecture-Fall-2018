.main:
    movu r0, 59999 @int a;
    mov r3, r0 @int value=a;
    movu r4, 0 @int digit=0;
    movu r5, 0 @int sum=0;
    .loop1:
        mod r4, r3, 10
        cmp r4, 0
        beq .check
        movu r6, 1
        mov r7, r4
        .loop2:
            mul r6, r6, r7
            sub r7, r7, 1   
            cmp r7, 1
            bgt .loop2            
        add r5, r5, r6
        div r3, r3, 10
        cmp r3, 0
        bgt .loop1
    cmp r5, r0
    beq .if1
    .dudeney:
        movu r5, 0 @sum=0
        movu r4, 0 @digit=0
        mov r3, r0 @value=a;
        .loop3:
            mod r4, r3, 10 @digit=value%10
            add r5, r5, r4 @sum=sum+digit
            div r3, r3, 10 @value=value/10
            cmp r3, 0 @ while value>0
            bgt .loop3
        mov r3, r5 @int new=sum;
        mul r5, r5, r3 @sum=sum*new;
        mul r5, r5, r3 @sum=sum*new;
        cmp r5, r0
        beq .if2
        .end:
            .print r1
            .print r2
            b .return
.check:
    add r3, r3, 1
    b .loop1
.if1:
    mov r1, 1
    b .dudeney
.if2:
    mov r2, 1
    b .end
.return:
@CS17BTECH11022