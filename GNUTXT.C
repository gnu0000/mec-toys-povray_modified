#include <process.h>
#include <dos.h>
#include "frame.h"
#include "povproto.h"
#include "povray.h"
#include "optin.h"
#include "msdosall.h"
#include <gnutype.h>
#include "gnucmd.h"

void MSDOS_Text_Init_Vars()
   {
   }

void MSDOS_Finish(int n)
   {
   exit(n);
   }

void MSDOS_Fatal(char *s)
   {
   printf ("%s", s);
   }

void MSDOS_Warning(char *s)
   {
   if (VERBOSE_LEVEL & 0x02)
      printf ("%s", s);
   }

void MSDOS_Status_Info(char *s)
   {
   if (VERBOSE_LEVEL & 0x04)
      printf ("%s", s);

   fflush (stdout);
   }

void MSDOS_Statistics(char *s)
   {
   if (VERBOSE_LEVEL & 0x08)
      printf ("%s", s);
   }

void MSDOS_Render_Info(char *s)
   {
   if (VERBOSE_LEVEL & 0x10)
      printf ("%s", s);
   }

void MSDOS_Banner(char *s)
   {
   if (VERBOSE_LEVEL & 0x20)
      printf ("%s", s);
   }

void MSDOS_Debug_Info(char *s)
   {
   if (VERBOSE_LEVEL & 0x40)
      printf ("%s", s);
   }
  
void MSDOS_Wait_Key(void)
   {
   MSDOS_getch();
   }

int MSDOS_getch(void)
   {
   int Key;
  
   if((Key=getch())==0)
      Key=256+getch();
   return(Key);
   }

void MSDOS_Test_Abort (void)
   {
   if (!(opts.Options & EXITENABLE))
      return;

   if (!kbhit()) 
      return;
   MSDOS_getch();
   Stop_Flag = TRUE;
   }

void MSDOS_Save_Text(void)
   {
   }

void MSDOS_Restore_Text(void)
   {
   }

int MSDOS_System(char *s)
   {
   int Code;
   int i;
   char *ARGS[20];
   char *t;
   char buf[POV_MAX_CMD_LENGTH];

   Render_Info("\nExecuting '%s'",s);
   strcpy(buf,s);
   t = ARGS[0] = strtok(buf, " ");
   i=1;
   while ((t!=NULL)&&(i<20))
      {
      t = ARGS[i] = strtok(0, " ");
      i++;
      }
   fprintf(stderr,"\nPOV-Ray shelling out to dos.\n%s\n",s);
   Code=spawnvp(P_WAIT,ARGS[0],ARGS);
   return(Code);
   }

void MSDOS_Process_Povray_Ini(char *s)
   {
   }

void MSDOS_Process_Env(void)
   {
   }

void MSDOS_Other_Credits(void)
   {
   }

#ifdef __WATCOMC__                        /* Watcom C/C++ C32 */
void Fix_Watcom_Bug(char *s)
{
  char *p=s;
  
  while(*p != '\0')
  {
    if (*p==' ')
    {
      *p='0';
    }
    p++;
  }  
}
#endif
