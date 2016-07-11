#include "saver.h"

void Saver::work()
try {
    writeIntoSheet();
    writeIntoFile();
    emit successSave();
}
catch (UpdateError e)
{
    emit smallError(e);
}
catch (...)
{
    emit smallError(UpdateError("Saving error","Can't save file."));
}


void Saver::writeIntoSheet()
{

}

void Saver::writeIntoFile()
{
#ifdef WIN32
    writer->write(sheet,name.utf16());
#else
    writer->write(sheet,name.toLatin1().data());
#endif
}
