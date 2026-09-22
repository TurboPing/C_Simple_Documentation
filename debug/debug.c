/* ----------------------------------------------------------------------------
 * debug.c
 * ----------------------------------------------------------------------------
 */

int
proc() {

	printf("i am in proc\n");

}

int
proc2() {

	printf("i am in proc2\n");

}


int
main(int argc, char **argv)
{
	
	proc();

	proc2();
		
	exit(0);
	
} 

