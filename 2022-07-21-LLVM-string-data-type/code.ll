@.str = unnamed_addr constant [6 x i8] c"Hello\00"

declare i32 @puts(i8* nocapture) nounwind

define i32 @main() {
  %cast210 = getelementptr [6 x i8], [6 x i8]* @.str, i64 0, i64 0

  call i32 @puts(i8* %cast210)
  ret i32 0
}
