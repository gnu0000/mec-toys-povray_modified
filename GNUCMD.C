#include <ctype.h>
#include <time.h>
#include <GnuType.H>
#include <GnuArg.H>
#include <GnuDir.H>
#include "frame.h"
#include "povproto.h"
#include "bbox.h"
#include "lighting.h"
#include "mem.h"        /*POV_FREE*/
#include "octree.h"
#include "povray.h"
#include "optin.h"
#include "optout.h"
#include "parse.h"
#include "radiosit.h"
#include "render.h"
#include "tokenize.h"
#include "vlbuffer.h"
#include "ppm.h"
#include "targa.h"
#include "userio.h"
#include "png_pov.h"
#include "msdosall.h"
#include "GnuCmd.h"

DBL fSIRDS = 0.0;

int bFIFTYLINES;
int VERBOSE_LEVEL;
int bPARSEONLY;

void  GnuUsage (void)
   {
   printf ("USAGE:  POVRAY [options] InFile [OutFile] [@inifile]             %s\n", __DATE__);
   printf ("WHERE:  Options are 0 or more of the following:                  Ver: %s\n", POV_RAY_VERSION);
   printf ("                                                                 Modified By CF\n");
   printf ("   Parameter       Default Description       Range   \n");
   printf ("   -------------------------------------------------------------------------\n");
   printf ("   /Help .............     Longer version of this Help\n");
   printf ("   /Width:# ..........640  Image Width.      (1-4096)\n");
   printf ("   /Height:# .........480  Image Height.     (1-4096)\n");
   printf ("   /Verbose:# ........15   Verbose level (see /Help for more info)\n");
   printf ("   /Alias:#[a|b] .....0.3  Anti-aliasing.    (alias:1.0 means no antialiasing)\n");
   printf ("   /Rays:# ...........3    Set Antialias depth.\n");
   printf ("   /Jitter:#..........1.0  Set Antialiasing Amount\n");
   printf ("   /Quality:# ........9    Image Quality.    (0=fastest, 9=best quality)\n");
   printf ("   /Radiosity ........     Apply Radiosity to image\n");
   printf ("   /Format:fmat ......T    Format. [T]arga, [P]pm p[N]g [C]omptarga [X]none\n");
   printf ("   /Output_Alpha .....     Write alpha channel (Targa, PNG)\n");
   printf ("   /Bits_Per_Color ...     only used by PNG format only?\n");
   printf ("   /Histogram_Type ...     [C]csv,[T]targa,[N]png,[P]ppm,[S]sys\n");
   printf ("   /Create_Ini .......     write an ini file\n");
   printf ("   /Show .............no   Display Image while rendering.\n");
   printf ("   /Display:Hex ......0    Set Display Type. (Hex Digit, 0=Autodetect)\n");
   printf ("   /Palette:PalChar ..3    Set Palette Type. (3,h,0,G,T)\n");
   printf ("   /Gamma=# ..........2.2  Set gamma value for display image\n");
   printf ("   /Draw_Vistas ......     Draw vista bounding boxes\n");
   printf ("   /Continue .........no   Continue aborted trace.\n");
   printf ("   /Preview[=n].......     Pixelwise preview\n");
   printf ("   /NoPause ..........no   Skip Pause when done (in Show mode).\n");
   printf ("   /NoBreak ..........no   Disallow exit via keypress.\n");
   printf ("   /Parse ............no   Do parse step only.\n");
   printf ("   /WriteIni=file ....no   Write out a (old) compatible ini file.\n");
   printf ("   /Library:path ......    Includes Path.    (defailt is current dir)\n");
   printf ("   /Buffer:# .........none Output Buffer Size in K.  (0=flush every line)\n");
   printf ("   /Version:#.........2.0  set version compatibility\n");
   printf ("   /Debug ............no   set debug trace\n");
   printf ("   /SIRDS:# ..........no   Generate output for SIRDS\n");
   printf ("   /50 ...............     leave at 50 line mode\n");
   printf ("   /SR:#  /ER:# ......0 n  set starting, ending row   \n");
   printf ("   /SC:#  /EC:# ......0 n  set starting, ending col   \n");
   printf ("   /Clock:#...........0.0  set the clock value\n");
   printf ("   /Frames:# .........     Set # animation frames\n");
   printf ("   /Initial_Clock:#...     clock at 1st frame\n");
   printf ("   /Final_Clock:# ....     clock at last frame\n");
   printf ("   /FrameStart:# .....     Start at this frame\n");
   printf ("   /FrameEnd:# .......     End at this frame\n");

   printf ("   /Cyclic=T|F .......T    on last anim clock<1 to match up\n");
   exit (0);
   }




void  GnuLongUsage (void)
   {
   printf ("USAGE:  POVRAY [options] InFile [OutFile] [@inifile]             %s\n", __DATE__);
   printf ("                                                                 Ver: %s\n\n", POV_RAY_VERSION);
   printf ("\n");
   printf ("Parameters may be switched with a '/' or a '-'.\n");
   printf ("Input file and output file (if specified) is not switched.\n");
   printf ("When a paarameter requires a value, an '=' or a ':' can be used.\n");
   printf ("\n");
   printf ("Parameters nead only be specified enough to make them unique.  For example\n");
   printf ("Al=3 is equivalent to Alias=3.\n");
   printf ("\n");
   printf ("In addition to the command line, params may be specified in env vars:\n");
   printf ("\n");
   printf ("   PVOPT0 .. This is scanned BEFORT the cmd line\n");
   printf ("   PVOPT  .. This is scanned  after the cmd line\n");
   printf ("   PVOPT1 .. This is scanned  after PVOPT\n");
   printf ("\n");
   printf ("These environment vars may contain params in the same format as the command\n");
   printf ("line.  If a param is given more than once, the 1st instance is used.\n");
   printf ("\n");
   printf ("   PVLIB  .. This is specifically to specift the path to the include dir.\n");
   printf ("             This is not a param.  Simply put in the path\n");
   printf ("\n");
   printf ("   ini files may be specified on the command line using a @ as the switch\n");
   printf ("   char.  Note that the params in these ini files must be in the original\n");
   printf ("   format, not the format described here.  this is for compatibility.\n");
   printf ("\n");
   printf ("-------------------------------------------------------------------------\n");
   printf ("Options are 0 or more of the following:\n\n");
   printf ("Parameter       Default Description       Range\n");
   printf ("-------------------------------------------------------------------------\n");
   printf ("/Help .............     This long Help\n");
   printf ("\n");
   printf ("/Width:# ..........640  Image Width.     (1-4096)\n");
   printf ("/X:#                    Another way of specifying Width\n");
   printf ("/Height:# .........480  Image Height.     (1-4096)\n");
   printf ("/Y:#                    Another way of specifying height\n");
   printf ("\n");
   printf ("/Verbose:# ........15   Verbose level. determines which messages you want\n");
   printf ("                        to see. Add the values to get the appropriate \n");
   printf ("                        message set.  The values are:\n");
   printf ("                           1  - Error Messages\n");
   printf ("                           2  - Warning Messages\n");
   printf ("                           4  - Status line while rendering\n");
   printf ("                           8  - Statistics after render\n");
   printf ("                           16 - Render info\n");
   printf ("                           32 - Banner info\n");
   printf ("                           64 - Debug messages\n");
   printf ("\n");
   printf ("/Alias:#[a|b] .....0.3a Anti-aliasing.    (alias:1.0 means no antialiasing)\n");
   printf ("                        a = default method, rays shot at pixel given by /Rays.\n");
   printf ("                        b = Recursion method recursion levels given by /Rays\n");
   printf ("                        Set low to something like .1 for final trace \n");
   printf ("/Rays:# ...........3    Set Antialias depth. See Alias (1 - 9)\n");
   printf ("/Jitter:#..........1.0  Set Antialiasing Amount (0-1) turn off for anim\n");
   printf ("/Quality:# ........9    Image Quality.    (0=fastest, 9=best quality)\n");
   printf ("/Radiosity ........     Apply Radiosity to image\n");
   printf ("\n");
   printf ("/Format:fmat ......T    Output Image Format. Choices Are:\n");
   printf ("                         T  targa       - may use Output_Alpha\n");
   printf ("                         C  compr targa - may use Output_Alpha\n");
   printf ("                         N  png         - may use Output_Alpha,Bits_Per_Color\n");
   printf ("                         P  ppm\n");
   printf ("                         X  No output\n");
   printf ("/Output_Alpha .....     Write alpha channel (Targa and PNG formats)\n");
   printf ("/Bits_Per_Color ...     only used by PNG format? (5 to 16 bits)\n");
   printf ("\n");
   printf ("/Histogram_Type ...X    If specified, a histogram file is written to\n");
   printf ("                        HISTGRAM.XXX  Allowable formats are:\n");
   printf ("                         T  targa\n");
   printf ("                         C  CSV\n");
   printf ("                         N  png\n");
   printf ("                         P  ppm\n");
   printf ("                         X  none (turn off)\n");
   printf ("\n");
   printf ("/Create_Ini .......     writes an ini file\n");
   printf ("\n");
   printf ("/Show .............no   Display Image while rendering.\n");
   printf ("/Display:Hex ......0    Set Display Type. (Hex Digit, 0=Autodetect)\n");
   printf ("                         0 - Auto detect (S)VGA type (Default)\n");
   printf ("                         1 - Standard VGA 320x200\n");
   printf ("                         2 - Standard VGA 360 x 480\n");
   printf ("                         3 - Tseng Labs 3000 SVGA 640x480\n");
   printf ("                         4 - Tseng Labs 4000 SVGA \n");
   printf ("                         5 - AT&T VDC600 SVGA 640x400\n");
   printf ("                         6 - Oak Technologies SVGA 640x480\n");
   printf ("                         7 - Video 7 SVGA 640x480\n");
   printf ("                         8 - Video 7 Vega (Cirrus) VGA 360x480\n");
   printf ("                         9 - Paradise SVGA 640x480\n");
   printf ("                         A - Ahead Systems Ver. A SVGA 640x480\n");
   printf ("                         B - Ahead Systems Ver. B SVGA 640x480\n");
   printf ("                         C - Chips & Technologies SVGA 640x480\n");
   printf ("                         D - ATI SGVA (older version) 640x480\n");
   printf ("                         E - Everex SVGA 640x480\n");
   printf ("                         F - Trident SVGA 640x480\n");
   printf ("                 use ->  G - VESA Standard SVGA Adapter\n");
   printf ("                         H - ATI XL display card\n");
   printf ("                         I - Diamond Computer Systems SpeedSTAR 24X\n");
   printf ("\n");
   printf ("/Palette:PalChar ..3    Set Palette Type. (3,h,0,G,T)\n");
   printf ("                         3 - 332 palette with dither\n");
   printf ("                         0 - HSV palette\n");
   printf ("                         G - Gray scale palettte\n");
   printf ("                         H - Hicolor with dither. Supported on VESA, \n");
   printf ("                             SpeedSTAR 24X, ATI XL HiColor and Tseng 4000\n");
   printf ("                         T - Truecolor. Supported on the Diamond \n");
   printf ("                             SpeedSTAR 24X and cards with 24bit VESA \n");
   printf ("                             support only.\n");
   printf ("\n");
   printf ("/Gamma=# ..........2.2  Set gamma value for display image.\n");
   printf ("/Draw_Vistas ......no   Draw vista bounding boxes. Has no effect on image\n");
   printf ("/Continue .........no   Continue aborted trace.\n");
   printf ("/Preview[=n].......     Pixelwise preview.  This will successively draw the\n");
   printf ("                        image sharper and sharper.  Optionally you can set \n");
   printf ("                        the initial pixel size.  id no # given 16 is used.\n");
   printf ("\n");
   printf ("/NoPause ..........no   Skip Pause when done (in Show mode).\n");
   printf ("/NoBreak ..........no   Disallow exit via keypress.\n");
   printf ("\n");
   printf ("/Parse ............no   Do parse step only.\n");
   printf ("/WriteIni=file ....no   Write out a (old) compatible ini file.\n");
   printf ("/Library:path ......    Includes Path. (defailt is current dir)\n");
   printf ("/Buffer:# .........no   Output Buffer Size in K.  (0=flush every line)\n");
   printf ("/Version:#.........3.0  set version compatibility\n");
   printf ("/Debug ............no   set debug trace\n");
   printf ("/SIRDS:# ..........no   Generate output for SIRDS # is cycle depth.\n");
   printf ("/50 ...............     leave at 50 line mode on exit from Show mode.\n");
   printf ("\n");
   printf ("/SR:#  /ER:# ......0    set starting, ending row for partial trace\n");
   printf ("                        Number can be pixen # or percent if <1\n");
   printf ("/SC:#  /EC:# ......0    set starting, ending col for partial trace\n");
   printf ("                        Number can be pixen # or percent if <1\n");
   printf ("\n");
   printf ("/Clock:#...........0.0  Set clock value.  If using Frames this is \n");
   printf ("                        automatically incremented from 0 to 1 thru frames\n");
   printf ("/Frames:# .........no   Set # animation frames. n images will be created\n");
   printf ("                        Filenames will have frame # appended.\n");
   printf ("/Initial_Clock ....     clock at 1st frame if using Frames\n");
   printf ("/Final_Clock ......     clock at last frame if using Frames\n");
   printf ("/FrameStart:# .....1    Start at this frame - for partial render of animation.\n");
   printf ("/FrameEnd:# .../frames  End at this frame - for partial render of animation.\n");
   printf ("/Cyclic=T|F .......T    For use with Frames.  If set to F the last frame\n");
   printf ("                        will have a clock value of 1.  If set to T the last\n");
   printf ("                        frame will have a value <1 so that it wont be\n");
   printf ("                        identical to the 1st frame for cyclic animation\n");
   exit (0);
   }


void *pov_dup (PSZ psz)
   {
   PSZ ptmp;

   ptmp = (char *)POV_MALLOC(strlen(psz)+1,"pov_dup");
   strcpy (ptmp, psz);
   return ptmp;
   }



UINT ParseInit (int argc, char **argv)
   {
   /*--- Define Valid Arguments ---*/
   ArgBuildBlk (
         "*^Width% *^Height% ^X% ^Y%"
         " *^Alias% *^Rays% *^Jitter% *^Quality%"
         " *^Radiosity *^Verbose% *^Format% *^Help"
         " *^Output_Alpha% *^Bits_Per_Color% *^Histogram_Type%"
         " *^Create_Ini? *^Show *^Display% *^Palette%  *^LookAt%"
         " *^Gamma% ? *^Draw_Vistas *^Continue *^Preview?"
         " *^NoPause *^NoBreak *^Parse *^WriteIni% *^Library% *^Buffer%"
         " *^Version% *^Debug *^Beep *50 ^SR% ^ER% *^SIRDS%"
         " ^SC% ^EC% *^Clock% *^Initial_Clock% *^Final_Clock%"
         " *^FrameStart% *^FrameEnd% ^Frames% *^Cyclic% *^BreakTest%");

   /*--- Eat Cmd Line, Env Variables, and default params ---*/
   if (ArgFillBlk2 (getenv ("PVOPT0")))
      return Error ("PVOPT0 ERROR: %s", ArgGetErr ());

   if (ArgFillBlk (argv))
      return Error ("CMDLINE ERROR: %s", ArgGetErr ());

   if (ArgFillBlk2 (getenv ("PVOPT")))
      return Error ("PVOPT ERROR: %s", ArgGetErr ());

   if (ArgFillBlk2 (getenv ("PVOPT1")))
      return Error ("PVOPT1 ERROR: %s", ArgGetErr ());


   if (ArgIs ("Help"))
      GnuLongUsage ();

   if (ArgIs ("?") || argc<2)
      GnuUsage ();

   return 0;
   }



//BOOL IsTrue (PSZ pszOpt)
//   {
//   PSZ   psz;
//   CHAR  c;
//
//   PSZ pszOpt
//
//
//   if (!ArgIs (pszOpt))             //
//      return FALSE;                 // Option not specified
//   if (!(psz = ArgGet (pszOpt, 0))) //
//      return TRUE;                  // Option specified without a value
//
//   c = toupper (*psz);
//   if (c = 'F' || c == '0' || c == 'N')
//      return FALSE;
//
//   return TRUE;
//   }
//





void ParseOptions (void)
   {
   UINT i, uFreeParams;
   PSZ  p, pszDefExt, pszIn, pszOut;
   BOOL bUsesOldIni;


   bFIFTYLINES = ArgIs ("50");

   /*--- Digest Cmd Line and Env Variables ---*/
   if (ArgIs ("Width"))
      if (!(Frame.Screen_Width = atoi (ArgGet ("Width", 0))))
         Error ("Bad Width Value", ArgGet ("Width", 0));

   if (ArgIs ("Height"))
      if (!(Frame.Screen_Height = atoi (ArgGet ("Height", 0))))
         Error ("Bad Height Value: %s", ArgGet ("Height", 0));

   if (ArgIs ("X"))  // overrides Width
      if (!(Frame.Screen_Width = atoi (ArgGet ("X", 0))))
         Error ("Bad X (Width) Value", ArgGet ("X", 0));

   if (ArgIs ("Y"))  // overrides Height
      if (!(Frame.Screen_Height = atoi (ArgGet ("Y", 0))))
         Error ("Bad Y (Height) Value: %s", ArgGet ("Y", 0));

   VERBOSE_LEVEL = (ArgIs ("Frames") ? 7 : 15);
   if (ArgIs ("Verbose")) //
      VERBOSE_LEVEL = atoi (ArgGet("Verbose", 0));

   if (ArgIs ("Alias")) //
      {
      opts.Antialias_Threshold = atof (ArgGet ("Alias", 0));
      if (opts.Antialias_Threshold < 1)
         opts.Options |= ANTIALIAS;
      if (strchr (ArgGet ("Alias", 0), 'b') || strchr (ArgGet ("Alias", 0), 'B'))
         opts.Tracing_Method = 2;
      else
         opts.Tracing_Method = 1;
      }

   if (ArgIs ("Rays")) //
      {
      opts.AntialiasDepth = atoi (ArgGet ("Rays", 0));
      opts.AntialiasDepth = min (9L, max (1L, opts.AntialiasDepth));
      }

   if (ArgIs ("Jitter")) //
      {
      opts.JitterScale = atof (ArgGet ("Jitter", 0));
      if (opts.JitterScale > 0.0)
         opts.Options |= JITTER;
      }

      
   if (ArgIs ("Quality")) //
      {
      opts.Quality = atoi (ArgGet ("Quality", 0));
      opts.Quality_Flags = Quality_Values[opts.Quality];
      }

   if (ArgIs ("Radiosity")) //
      opts.Options |= RADIOSITY; 

   if (ArgIs ("Format")) //
      {
      opts.OutputFormat = (char)tolower (*ArgGet ("Format", 0));

      if (!strchr ("cnpstx", opts.OutputFormat))
         Error ("Bad File Format: %s", ArgGet ("Format", 0));
      if (opts.OutputFormat != 'x')
         opts.Options |= DISKWRITE;
      }
   else
      {
      opts.OutputFormat = 't';
      opts.Options |= DISKWRITE;
      }

   if (ArgIs ("Output_Alpha")) //
      opts.Options |= OUTPUT_ALPHA;

   if (ArgIs ("Bits_Per_Color")) //
      opts.OutputQuality = min(16, max(5, atoi(ArgGet ("Bits_Per_Color", 0))));

   if (ArgIs ("Show")) //
      {
      opts.Options |= DISPLAY;
      opts.Options &= ~VERBOSE;

      if (ArgIs ("NoPause"))
         opts.Options &= ~PROMPTEXIT;
      else
         opts.Options |= PROMPTEXIT;
      }
   else
      {
      opts.Options |= VERBOSE;
      }


   if (ArgIs ("Display")) //
      opts.DisplayFormat = (char) toupper (*ArgGet ("Display", 0));

   if (ArgIs ("Palette")) //
      opts.PaletteOption = (char) toupper (*ArgGet ("Palette", 0));

// if (ArgIs ("LookAt"))

   if (ArgIs ("Gamma")) //
      opts.DisplayGamma = atof (ArgGet ("Gamma", 0));
      

   if (ArgIs ("Draw_Vistas")) //
      opts.Options |= USE_VISTA_DRAW;

   if (ArgIs ("Continue")) //done
      opts.Options |= CONTINUE_TRACE;

   if (ArgIs ("NoBreak")) //
      opts.Options &= ~EXITENABLE;
   else
      {
      opts.Options |= EXITENABLE;
      /*--- clear kbd buffer ---*/
      while (kbhit())
         MSDOS_getch();
      }

   if (ArgIs ("BreakTest"))
      opts.Abort_Test_Counter = atoi (ArgGet ("BreakTest", 0));
   else
      opts.Abort_Test_Counter = 50;

   if (ArgIs ("Library")) //
      for (i=0; ArgIs ("Library") > i; i++, opts.Library_Path_Index++)
         if (opts.Library_Path_Index >= MAX_LIBRARIES)
            Error ("Too many library directories specified.", "");
         else
            opts.Library_Paths[opts.Library_Path_Index] = pov_dup (ArgGet ("Library", i));

   if (p = getenv ("PVLIB"))
      {
      if (opts.Library_Path_Index >= MAX_LIBRARIES)
        Error ("Too many library directories specified.", p);
      else
        opts.Library_Paths[opts.Library_Path_Index++] = pov_dup (p);
      }

   if (ArgIs ("Buffer")) //done
      {
      opts.File_Buffer_Size = atoi (ArgGet ("Buffer", 0)) * 1024;
      opts.File_Buffer_Size = min (MAX_BUFSIZE, max (BUFSIZ, opts.File_Buffer_Size));
      if (opts.File_Buffer_Size)
         opts.Options |= BUFFERED_OUTPUT;
      }

   if (ArgIs ("Version"))
      opts.Language_Version = atof (ArgGet ("Version", 0));

//   if (ArgIs ("Debug"))
//      opts.Options |= DEBUGGING;

   if (ArgIs ("SR")) //
      if (atof (ArgGet ("SR", 0)) > 1.0)
         opts.First_Line = atoi (ArgGet ("SR", 0));
      else
         opts.First_Line = (int) (Frame.Screen_Height * atof (ArgGet ("SR", 0)));

   if (ArgIs ("ER")) //
      if (atof (ArgGet ("ER", 0)) > 1.0)
         opts.Last_Line = atoi (ArgGet ("ER", 0));
      else
         opts.Last_Line = (int) (Frame.Screen_Height * atof (ArgGet ("ER", 0)));

   if (ArgIs ("SC")) //
      if (atof (ArgGet ("SC", 0)) > 1.0)
         opts.First_Column = atoi (ArgGet ("SC", 0));
      else
         opts.First_Column = (int) (Frame.Screen_Width * atof (ArgGet ("SC", 0)));

   if (ArgIs ("EC")) //
      if (atof (ArgGet ("EC", 0)) > 1.0)
         opts.Last_Column = atoi (ArgGet ("EC", 0));
      else
         opts.Last_Column = (int) (Frame.Screen_Width * atof (ArgGet ("EC", 0)));

   if (ArgIs ("Clock")) //
      opts.FrameSeq.Clock_Value = atof (ArgGet ("Clock", 0));

   if (ArgIs ("Frames")) //
      opts.FrameSeq.FinalFrame = atoi (ArgGet ("Frames", 0));

   if (ArgIs ("FrameStart")) //
      opts.FrameSeq.SubsetStartFrame = atoi (ArgGet ("FrameStart", 0));

   if (ArgIs ("FrameEnd")) //
      opts.FrameSeq.SubsetEndFrame = atoi (ArgGet ("FrameEnd", 0));

   if (ArgIs ("Initial_Clock")) //
        opts.FrameSeq.InitialClock= atof (ArgGet ("Initial_Clock", 0));
      
   if (ArgIs ("Final_Clock")) //
        opts.FrameSeq.FinalClock= atof (ArgGet ("Final_Clock", 0));


   opts.Options |= CYCLIC_ANIMATION;
   if (ArgIs ("Cyclic")) //
      {
      if (*ArgGet ("Cyclic", 0) == 'T' || *ArgGet ("Cyclic", 0) == 't')
         opts.Options |= CYCLIC_ANIMATION;
      else
         opts.Options &= ~CYCLIC_ANIMATION;
      }

   if (ArgIs ("Preview")) //
      {
      if (ArgGet ("Preview", 0))
         opts.PreviewGridSize_Start = atoi(ArgGet ("Preview", 0));
      else
         opts.PreviewGridSize_Start = 16;
      opts.PreviewGridSize_End = 1;
      }

   bPARSEONLY = ArgIs ("Parse");

   if (ArgIs ("SIRDS"))
      fSIRDS = atof (ArgGet ("SIRDS", 0));

   if (ArgIs ("Histogram_Type"))
      {
      switch (toupper (*ArgGet ("Histogram_Type", 0)))
         {
         case 'C': 
            opts.histogram_type = CSV ;   
            pszDefExt = ".csv";
            opts.histogram_on = TRUE ;
            break;
         case 'S': 
            opts.histogram_type = SYS ;   
            pszDefExt = SYS_DEF_EXT;
            Histogram_File_Handle = GET_SYS_FILE_HANDLE () ;
            opts.histogram_on = TRUE ;
            break;
         case 'P': 
            opts.histogram_type = PPM ;   
            pszDefExt = ".ppm";
            Histogram_File_Handle = Get_PPM_File_Handle () ;
            opts.histogram_on = TRUE ;
            break;
         case 'N': 
            opts.histogram_type = PNG ;   
            pszDefExt = ".png";
            Histogram_File_Handle = Get_Png_File_Handle () ;
            opts.histogram_on = TRUE ;
            break;
         case 'T': 
            opts.histogram_type = TARGA ; 
            pszDefExt = ".tga";
            Histogram_File_Handle = Get_Targa_File_Handle () ;
            opts.histogram_on = TRUE ;
            break;
         case 'X': 
            opts.histogram_on = FALSE ;
            break;
        default :
            opts.histogram_type = TARGA ; 
            pszDefExt = ".tga";
            Histogram_File_Handle = Get_Targa_File_Handle () ;
            opts.histogram_on = TRUE ;
         }
      opts.histogram_on = TRUE ;
      sprintf(opts.Histogram_File_Name, "histgram%s", pszDefExt);
      }


   if (ArgIs ("WriteIni"))
      strcpy(opts.Ini_Output_File_Name, ArgGet ("WriteIni", 0));

   /*--- File names handled last ---*/
   uFreeParams = ArgIs (NULL);
   pszIn = pszOut = NULL;
   bUsesOldIni = FALSE;
   for (i=0; i<uFreeParams; i++)
      {
      p = ArgGet (NULL, i);

      if (*p == '@')
         {
         if (!parse_ini_file(p+1))
            Error ("Could not open Include_Ini='%s'.\n", p+1);
         bUsesOldIni = TRUE;
         continue;
         }
      if (*p == '?')
         GnuUsage ();
      if (!pszIn)
         pszIn = p;
      else if (!pszOut)
         pszOut = p;
      else
         Error ("Cannot have more than 2 filenames: %s", p);
      }

   if (!bUsesOldIni)
      {
      if (!pszIn)
         Error ("No source file specified", "");
      strcpy (opts.Input_File_Name, pszIn);
      }
   if (!strchr (opts.Input_File_Name, '.'))
      strcat (opts.Input_File_Name, ".pov");

   if (!(opts.Options & DISKWRITE))
      return;

   switch (opts.OutputFormat)
      {
      case 'c': pszDefExt = ".tga";  break;
      case 't': pszDefExt = ".tga";  break;
      case 'n': pszDefExt = ".png";  break;
      case 'p': pszDefExt = ".ppm";  break;
      case 's': pszDefExt = SYS_DEF_EXT;  break;
      }

   if (pszOut)
      strcpy (opts.Output_File_Name, pszOut);
   else if (!bUsesOldIni || !*opts.Output_File_Name)
      DirMakeFileName (opts.Output_File_Name, NULL, opts.Input_File_Name, pszDefExt);

   if (!strrchr (opts.Output_File_Name, '.'))
      strcat (opts.Output_File_Name, pszDefExt);

   if (!stricmp (opts.Output_File_Name, opts.Input_File_Name))
      {
      fprintf (stderr, "Error InputFile == OutputFile\n");
      exit (1);
      }

   if (ArgIs ("Create_Ini"))
      DirMakeFileName (opts.Ini_Output_File_Name, NULL, opts.Input_File_Name, ".ini");

   if (ArgIs ("Debug"))
      ArgDump ();
   }
   
