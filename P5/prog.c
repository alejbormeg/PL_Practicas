#include <stdlib.h>
#include <stdio.h>
#include "dec_dat.h"

#include "dec_fun.h"
int main()
{
	{
		{
			int n;
			Lista pares, impares, suma, doble, concatenacion;
			
			int imprimirLista2(Lista l)
			{
				{
					printf("%s", listaAString(l));
					fflush(stdout);
					printf("\n");
				}
				{
					return 0;
				}
			}

			Lista obtenerPares(int n)
			{
				int i;
				Lista pares;
				
				int imprimirLista(Lista l)
				{
					{
						printf("%s", "Lista pares\n");
						fflush(stdout);
						printf("\n");
					}
					{
						printf("%s", listaAString(l));
						fflush(stdout);
						printf("\n");
					}
					{
						return 0;
					}
				}

				{
					i = 1;
				}
				{
					Lista temp1;
					temp1 = inicializar(tInt);
					insertar(temp1, pInt(0));
					pares = temp1;
				}
				{
					etiqueta2:
					{
						{
							int temp2;
							temp2 = i < n;
							
							if (!temp2) goto etiqueta1;
						}

						{
							{
								{
									{
										Lista temp3;
										temp3 = inicializar(tInt);
										insertar(temp3, pInt(i));
										Lista temp4;
										temp4 = concatenar(pares, temp3);
										pares = temp4;
									}
									{
										int temp5;
										temp5 = i + 1;
										i = temp5;
									}
								}

							}
						}

						goto etiqueta2;

					}
					etiqueta1: {}
				}
				{
					Lista temp6;
					temp6 = multiplicarLista(pares, 2);
					pares = temp6;
				}
				{
					imprimirLista(pares);
				}
				{
					return pares;
				}
			}

			Lista obtenerImpares(int n)
			{
				int i;
				Lista impares;
				
				int imprimirLista(Lista l)
				{
					{
						printf("%s", "Lista impares\n");
						fflush(stdout);
						printf("\n");
					}
					{
						printf("%s", listaAString(l));
						fflush(stdout);
						printf("\n");
					}
					{
						return 0;
					}
				}

				{
					i = 1;
				}
				{
					Lista temp7;
					temp7 = inicializar(tInt);
					insertar(temp7, pInt(1));
					impares = temp7;
				}
				{
					etiqueta4:
					{
						{
							int temp8;
							temp8 = 2 * n;
							int temp9;
							temp9 = temp8 - 1;
							int temp10;
							temp10 = i < temp9;
							
							if (!temp10) goto etiqueta3;
						}

						{
							{
								{
									{
										int temp11;
										temp11 = i + 2;
										i = temp11;
									}
									{
										Lista temp12;
										temp12 = inicializar(tInt);
										insertar(temp12, pInt(i));
										Lista temp13;
										temp13 = concatenar(impares, temp12);
										impares = temp13;
									}
								}

							}
						}

						goto etiqueta4;

					}
					etiqueta3: {}
				}
				{
					imprimirLista(impares);
				}
				{
					return impares;
				}
			}

			{
				printf("%s", "Introduzca un número: ");
				fflush(stdout);
				printf("\n");
			}
			{
				scanf("%d", &n);
			}
			{
				pares = obtenerPares(n);
			}
			{
				impares = obtenerImpares(n);
			}
			{
				Lista temp14;
				temp14 = sumarLista(pares, 100);
				suma = temp14;
			}
			{
				printf("%s", "\nSuma de la lista de los pares + 100: ");
				fflush(stdout);
				printf("\n");
			}
			{
				imprimirLista2(suma);
			}
			{
				Lista temp15;
				temp15 = multiplicarLista(pares, 2);
				doble = temp15;
			}
			{
				printf("%s", "\nMultiplicacion * 2 de la lista de los pares: ");
				fflush(stdout);
				printf("\n");
			}
			{
				imprimirLista2(doble);
			}
			{
				Lista temp16;
				temp16 = concatenar(pares, impares);
				concatenacion = temp16;
			}
			{
				printf("%s", "\nConcatenacion de pares e impares: ");
				fflush(stdout);
				printf("\n");
			}
			{
				imprimirLista2(concatenacion);
			}
		}

	}
}

