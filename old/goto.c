#include <stdio.h>

int function(void)
{
	goto end;
	return 1;

end:
	return 0;
} 

int main()
{
	printf("%d\n", function());
	goto end;
	return 1;

end:
	return 0;
}
