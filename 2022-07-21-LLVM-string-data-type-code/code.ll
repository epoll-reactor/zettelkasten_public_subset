; ModuleID = 'hello.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-linux-gnu"
@.str = internal constant [14 x i8] c"Hello, world!\00"         ; <[14 x i8]*> [#uses=1]

define i32 @main() nounwind  {
entry:
        %tmp2 = tail call i32 @puts( i8* getelementptr ([14 x i8]* @.str, i32 0, i64 0) ) nounwind              ; <i32> [#uses=0]
        ret i32 0
}

declare i32 @puts(i8*)
