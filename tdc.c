/* tiny dc clone */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int
main()
{
	/* tstack is used for temporary values... */
	float stack[25]; 
	//char buf[20];
	int sp = -1, in = 0, temp = 0;
	printf("A tiny dc clone\n");
	while((in = getc(stdin)) != -1)
	{
		switch(in)
		{
			case 'p': /* print the top */
				if(sp < 0)
					printf("Stack Underflow!\n");
				else
					printf("%f\n",stack[sp]);
				break;
			case 'q':
				return 0;
			case 'P': /* print and Pop */
				if(sp < 0)
					printf("Stack underflow!\n");
				else
					printf("%f\n",stack[sp]);
				--sp;
				break;
			case 'f': /* dump stack */
				printf("depth <%i>\n",sp + 1);
				for(temp = sp;temp >= 0;temp--)
					printf("  %f\n",stack[temp]);
				break;
			case 'c': /* clear */
				sp = -1;
				break;
			case 'r': /* swap, from GNU dc */
				if(sp < 1)
					printf("Stack underflow!\n");
				else
					temp = stack[sp];
					stack[sp] = stack[sp - 1];
					stack[sp - 1] = temp;	
				break;
			case 'd':
				if((sp + 1) >= 25)
					printf("Stack over flow!\n");
				else
					++sp;
					stack[sp] = stack[(sp - 1)];
				break;
			case 'D': /*depth*/
				if((sp + 1) >= 25)
					printf("Stack overflow!\n");
				else
				{
					++sp;
					stack[sp] = sp - 1;
				}
				break;
			case 'R': /* rotate top 3*/
		       		if(sp < 2)
					printf("Stack underflow!\n");
				else
				{
					temp = stack[sp - 1];
					stack[sp - 1] = stack[sp - 2];
					stack[sp - 2] = temp;
					temp = stack[sp];
					stack[sp] = stack[sp - 1];
					stack[sp - 1] = temp;
				}
				break;
			case 'N': /* nip */
				if(sp <= 0)
					printf("Stack underflow!\n");
				else
				{
					temp = stack[sp];
					sp--;
					stack[sp] = temp;
				}
				break;
			case 'n': /* not */
				stack[sp] = - stack[sp];
				break ;
			case 'T': /* tuck */
				if(sp < 1)
					printf("Stack underflow!\n");	
				else if((sp + 1) >= 25)
					printf("Stack overflow!\n");
				else
				{
					stack[(sp + 1)] = stack[sp];
					stack[sp] = stack[(sp - 1)];
					stack[(sp - 1)] = stack[(sp + 1)];
					sp++;	
				}	
				break;
			case 'O': /* over */
				if(sp < 1)
					printf("Stack underflow!\n");
				else if(sp + 1 >= 25)
					printf("Stack overflow!\n");
				else
				{
					stack[(sp + 1)] = stack[(sp - 1)];
					sp++;
				}	
				break;
			case 'V': /* drop */
				if(sp < 0)
					printf("Stack underflow!\n");
				else
					sp--;
				break;	
			case '+': /* add two numbers */
				if(sp < 1)
					printf("Stack Underflow!\n");
				else
				{
					stack[(sp - 1)] += stack[sp];
					sp--;
				}
				break;
			case '-': /* subtract two numbers */
				if(sp < 1)
					printf("Stack underflow!\n");
				else
				{
					stack[(sp - 1)] -= stack[sp];
					sp--;
				}
				break;
			case '*': /*multiply two numbers */	
				if(sp < 1)
					printf("Stack underflow!\n");
				else
				{
					stack[(sp - 1)] *= stack[sp];
					sp--;
				}
				break;
			case '/': /* divide two numbers */
				if(sp < 1)
					printf("Stack underflow!\n");
				else if(stack[sp] == 0)
					printf("Divide by Zero\n");
				else
				{
					stack[(sp - 1)] /= stack[sp];
					sp--;
				}	
				break;	
			case '^': /* exponent */
				if(sp < 1)
					printf("Stack underflow!\n");
				else
				{
					stack[(sp - 1)] = pow(stack[(sp - 1)], stack[sp]);
					sp--;
				}
				break;	
			/*ignore whitespace */
			case '\t': break;
			case ' ': break;
			case '\n': break;
			default: /* add number recog here */
				if((in >= '0' && in <= '9') || in == '.')
				{
					ungetc(in,stdin);
					sp++;
					if(sp < 25)
						scanf("%f",&stack[sp]);
					else
					{
						printf("Stack overflow!");	
						return 1;
					}	
				}
				else
					printf("%c is not implemented!\n",in);
				break ;
		}
	}
	return 0;
}

