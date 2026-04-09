# CONTRIBUTION GUIDELINES
* Indents (size 4) use tabs (I know spaces are conventional, but I don't like them. Sorry).
* Spacing:
  	1. **Between parenthesis (with content) when the content is an expression, parameter/arguments list in definition, declaration, or call; control statement, or condition (No casts please).**
		* Example:
	 		## ✔️ CORRECT
			```
			void MyFunction( int x, int y )
   			{
				printf( "X: %d", x );
   				printf( "Y: %d", y );
   			}
   
			if ( x == y )
   			{
   				MyFunction( x, y );
   			}
			```
	  		## ✖️ INCORRECT
			```
			void MyFunction(int x, int y) {
				printf("X: %d", x);
   				printf("Y: %d", y);
   			}
   
			if (x == y)
   			{
   				MyFunction(x, y);
   			}
			```
   	2. **Between operators and commas.**
   		* Example:
			## ✔️ CORRECT
			```
   	 		int z = x + y;
   	 		printf( "%d", z );
			```
	  		## ✖️ INCORRECT
			```
			int z=x+y;
   	 		printf("%d",z);
			```
* Newlines between function/control blocks:
	* Example:
 		## ✔️ CORRECT
		```
		void MyFunction()
		{
		}
		```
  		## ✖️ INCORRECT
		```
		void MyFunction() {
		}
		```
There might be more but I'm probably forgetting. Feel free to use the `.editorconfig` file.
