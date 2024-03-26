; ModuleID = 'division-interpreter'
declare i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
	%calc = alloca i32

	store i32 0, i32* %calc

	%temp_var0 = sdiv i32 645 , 47
	%temp_var1 = sdiv i32 7500 , %temp_var0
	store i32 %temp_var1, i32* %calc
	%temp_var2 = load i32, i32* %calc
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %temp_var2)
	ret i32 0
}