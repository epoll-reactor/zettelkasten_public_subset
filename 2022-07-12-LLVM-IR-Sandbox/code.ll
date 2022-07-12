define i32 @main() {
entry:
  ; Если здесь поиграть с попаданием в разные блоки в br,
  ; можно увидеть с помощью strace, что после компиляции
  ; процесс вышел с разными кодами.
  %condition = icmp ne i32 1, 1
  br i1 %condition, label %then, label %else3

then:                                             ; preds = %entry
  %0 = add i32 1, 1
  %condition1 = icmp ne i32 %0, 0
  br i1 %condition1, label %then2, label %else

then2:                                            ; preds = %then
  ret i32 0
  br label %ifContinue

else:                                             ; preds = %then
  ret i32 1
  br label %ifContinue

ifContinue:                                       ; preds = %else, %then2
  br label %ifContinue8

else3:                                            ; preds = %entry
  %3 = add i32 1, 2
  %condition4 = icmp ne i32 %3, 0
  br i1 %condition4, label %then5, label %else6

then5:                                            ; preds = %else3
  ret i32 2
  br label %ifContinue7

else6:                                            ; preds = %else3
  ret i32 3
  br label %ifContinue7

ifContinue7:                                      ; preds = %else6, %then5
  br label %ifContinue8

ifContinue8:                                      ; preds = %ifContinue7, %ifContinue
  ret i32 4
}
