#ifndef GETTRANSLATE
#define GETTRANSLATE

#include <QString>

class GetTranslate
{

public:
    virtual void getTranslate(QString translate, QString origin, QString autoLng) = 0;
};

#endif // GETTRANSLATE

