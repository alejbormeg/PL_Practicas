#include <stdlib.h>
#include <stdio.h>
#include "dec_dat.h"

#include "dec_fun.h"
int main()
{
	{
		{
			int num_elementos, i;
			Lista resultado, por_dos, sumar;
			
			int fib(int count)
			{
				int a, b, c;
				int ctr;
				
				int imprimeDig(int dig)
				{
					int addToLista(int dig)
					{
						{
							Lista temp1;
							temp1 = inicializar(tInt);
							insertar(temp1, pInt(dig));
							Lista temp2;
							temp2 = concatenar(resultado, temp1);
							resultado = temp2;
						}
						{
							return 0;
						}
					}

					{
						printf("%d", dig);
						fflush(stdout);
						printf("%s", " ");
						fflush(stdout);
						printf("\n");
					}
					{
						addToLista(dig);
					}
					{
						return 0;
					}
				}

				{
					a = 1;
				}
				{
					b = 1;
				}
				{
					imprimeDig(a);
				}
				{
					imprimeDig(b);
				}
				{
					ctr = 2;
				}
				{
					etiqueta2:
					{
						{
							int temp3;
							temp3 = ctr < count;
							
							if (!temp3) goto etiqueta1;
						}

						{
							{
								int temp4;
								temp4 = a + b;
								c = temp4;
							}
							{
								a = b;
							}
							{
								b = c;
							}
							{
								int temp5;
								temp5 = ctr + 1;
								ctr = temp5;
							}
							{
								imprimeDig(c);
							}
						}

						goto etiqueta2;

					}
					etiqueta1: {}
				}
				{
					return 0;
				}
			}

			{
				printf("%s", "Introduzca el número de elementos a calcular:> ");
				fflush(stdout);
				printf("\n");
			}
			{
				scanf("%d", &num_elementos);
			}
			{
				printf("%s", "\nFibonacci normal es:\n");
				fflush(stdout);
				printf("\n");
			}
			{
				fib(num_elementos);
			}
			{
				Lista temp6;
				temp6 = multiplicarLista(resultado, 2);
				por_dos = temp6;
			}
			{
				printf("%s", "\n2*Fibonacci es:\n");
				fflush(stdout);
				printf("%s", listaAString(por_dos));
				fflush(stdout);
				printf("%s", "\n");
				fflush(stdout);
				printf("\n");
			}
			{
				inicio(resultado);
			}
			{
				inicio(por_dos);
			}
			{
				i = 0;
			}
			{
				etiqueta4:
				{
					{
						int temp7;
						temp7 = getTam(resultado);
						int temp8;
						temp8 = i < temp7;
						
						if (!temp8) goto etiqueta3;
					}

					{
						{
							int temp9;
							temp9 = *(int*)getActual(resultado);
							int temp10;
							temp10 = *(int*)getActual(por_dos);
							int temp11;
							temp11 = temp9 + temp10;
							Lista temp12;
							temp12 = inicializar(tInt);
							insertar(temp12, pInt(temp11));
							Lista temp13;
							temp13 = concatenar(sumar, temp12);
							sumar = temp13;
						}
						{
							int temp14;
							temp14 = getTam(resultado);
							int temp15;
							temp15 = temp14 - 1;
							int temp16;
							temp16 = i != temp15;
							
							if (!temp16) goto etiqueta5;
							{
								{
									{
										avanza(resultado);
									}
									{
										avanza(por_dos);
									}
								}

							}
							goto etiqueta6;

							etiqueta5: {}
							
							etiqueta6: {} 
						}
						{
							int temp17;
							temp17 = i + 1;
							i = temp17;
						}
					}

					goto etiqueta4;

				}
				etiqueta3: {}
			}
			{
				printf("%s", "3*Fibonacci es:\n");
				fflush(stdout);
				printf("%s", listaAString(sumar));
				fflush(stdout);
				printf("%s", "\n");
				fflush(stdout);
				printf("\n");
			}
		}

	}
}

