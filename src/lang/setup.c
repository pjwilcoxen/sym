#include "../lang.h"

void Debug_setup(void);
void Msgproc_setup(void);
void OxGS_setup(void);
void OxGST_setup(void);
void Oxnewton_setup(void);
void Troll_setup(void);
void Tablo_setup(void);
void Html_setup(void);
// GCS 2022-11-22 addition for Python version of GCubed
void Python_setup(void);

extern char *Debug_version;
extern char *Msgproc_version;
extern char *OxGS_version;
extern char *OxGST_version;
extern char *Oxnewton_version;
extern char *Troll_version;
extern char *Tablo_version;
extern char *Html_version;
// GCS 2022-11-22 addition for Python version of GCubed
extern char *Python_version;

void setup_languages()
{
   addlang("debug", Debug_setup, Debug_version);
   addlang("msgproc", Msgproc_setup, Msgproc_version);
   addlang("oxgs", OxGS_setup, OxGS_version);
   addlang("oxgst", OxGST_setup, OxGST_version);
   addlang("oxnewton", Oxnewton_setup, Oxnewton_version);
   addlang("troll", Troll_setup, Troll_version);
   addlang("tablo", Tablo_setup, Tablo_version);
   addlang("html", Html_setup, Html_version);
   // GCS 2022-11-22 addition for Python version of GCubed
   addlang("python", Python_setup, Python_version);
}
