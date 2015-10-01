/*****************************************************************************/
/* chat.c                                                    Septiembre 2012 */
/*   Sergio Bravo Martín                                                     */
/*   Ángeles Mª Moreno Montero                                               */
/*---------------------------------------------------------------------------*/
/* Este ejemplo ilustra la modalidad asíncrona de comunicación por el puerto */
/* serie a través del funcionamiento de una sencilla aplicación de Chat.     */
/* Todos los caracteres tecleados por el usuario son enviados por el puerto  */
/* serie y los caracteres recibidos son mostrados en la salida estándar.     */
/*                                                                           */
/* La configuración del puerto serie es: 9600 baudios, 8 bit de datos, 1 bit */
/* de parada y sin paridad.                                                   */
/*---------------------------------------------------------------------------*/
/* Ejemplo de uso:                                                           */
/*                                                                           */
/*   chat COM1                                                               */
/*                                                                           */
/* El puerto serie escogido debe pasarse como parámetro                      */
/*---------------------------------------------------------------------------*/
/* En windows, los nombres de los dispositivos serie son:                    */
/*                                                                           */
/*   COM1 --> Primer puerto serie                                            */
/*   COM2 --> Segundo puerto serie                                           */
/*   ...                                                                     */
/*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "seriewin.h"

//-----------------------------------------------------------------------------
//-- CONSTANTES
//-----------------------------------------------------------------------------

//-- Código ASCII de la tecla ESCape
#define ESC '\x1B'

//-- Número máximo de caracteres de la cadena
#define MAXCHARS 80

//-----------------------------------------------------------------------------
//-- PROTOTIPOS DE FUNCIONES
//-----------------------------------------------------------------------------
DWORD HiloLectura(LPDWORD lpdwParam);

/*****************************************************************************/
/* PROGRAMA PRINCIPAL                                                        */
/*****************************************************************************/
int main(int argc, char *argv[])
{
    HANDLE hCommPort;

    const char* config_file = ".\\serie.ini";

    unsigned int baud_rate = GetPrivateProfileInt("Configuracion",
                                                  "Velocidad",
                                                  9600,
                                                  config_file);

    unsigned int data_bits = GetPrivateProfileInt("Configuracion",
                                                  "BitsDatos",
                                                  8,
                                                  config_file);

    unsigned int stop_bits = GetPrivateProfileInt("Configuracion",
                                                  "BitsParada",
                                                  2,
                                                  config_file);

    char port_name[30];
    unsigned int _len = GetPrivateProfileString("Configuracion",
                                                "Puerto",
                                                "COM1",
                                                port_name,
                                                sizeof(port_name),
                                                config_file);

    char parity[20];
    unsigned int _parity_len = GetPrivateProfileString("Configuracion",
                                                       "Paridad",
                                                       "Sin paridad",
                                                       parity,
                                                       sizeof(parity),
                                                       config_file);

    printf("Datos: %d %d %d %s %s\n", baud_rate, data_bits, stop_bits, port_name, parity);

    //-- Abre el puerto serie
    hCommPort = serie_abrir(port_name, baud_rate, data_bits, stop_bits, parity);
    if (hCommPort == INVALID_HANDLE_VALUE)
    {
        perror("ERROR: No se puede acceder al puerto serie.");
        exit(2);
    }

    //-- Crea el hilo encargado de la lectura de datos del puerto serie
    HANDLE hHiloLectura;
    DWORD idHiloLectura;
    hHiloLectura = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HiloLectura, &hCommPort, 0, &idHiloLectura);
    if (hHiloLectura == NULL)
    {
        perror("ERROR: No se puede crear el hilo de lectura");
        exit(3);
    }

    printf("Pulse la tecla ESC para salir\n");

    //-- Bucle principal de escritura por el puerto serie
    char c;
    do
    {
        c = _getch();
        if (c != ESC) {
            serie_escribir(hCommPort, &c, sizeof(c));
        }
    } while (c != ESC);

    //-- Termina el hilo de lectura
    TerminateThread(hHiloLectura, 0);
    CloseHandle(hHiloLectura);

    //-- Cierra el puerto serie
    serie_cerrar(hCommPort);

    system("PAUSE");
    return 0;
}

/*****************************************************************************/
/* Función asociada al hilo encargado de leer datos del puerto serie y       */
/* mostrarlos por la salida estándar                                         */
/*---------------------------------------------------------------------------*/
/* ENTRADA:                                                                  */
/*   -lpParam: manejador del puerto serie (HANDLE hCommPort)                 */
/*                                                                           */
/*****************************************************************************/
DWORD HiloLectura(LPDWORD lpParam)
{
    DWORD dwEvtMask;

    OVERLAPPED ov;
    memset(&ov, 0, sizeof(ov));
    ov.hEvent = CreateEvent(0, TRUE, 0, 0);

    //-- Recoge el manejador del puerto serie
    HANDLE hCommPort = (HANDLE)*lpParam;

    char szDatos[MAXCHARS + 1];

    //-- Bucle infinito de lectura
    while (TRUE)
    {
        if (!WaitCommEvent(hCommPort, &dwEvtMask, &ov))
        {
            //-- perror("ERROR: WaitCommEvent. Esperando evento síncrono");
        }

        //-- Espera a que haya algo que leer
        if (WAIT_OBJECT_0 == WaitForSingleObject(ov.hEvent, INFINITE))
        {
            DWORD dwMask;
            if (GetCommMask(hCommPort, &dwMask))
            {
                if (dwMask == EV_TXEMPTY)
                {
                    ResetEvent(ov.hEvent);
                    continue;
                }
            }

            //-- Lee los datos del puerto serie
            if (serie_leer(hCommPort, szDatos, sizeof(szDatos)) > 0)
                printf("%s", szDatos);

            //-- Reinicia el estado del evento
            ResetEvent(ov.hEvent);
        }
    }

    return 0;
}
