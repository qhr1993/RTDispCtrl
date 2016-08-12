#include <QCoreApplication>
#include <QSharedMemory>
#include <QDebug>
#include "sharedcontrol.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


int main(int argc, char *argv[])
{
    QSharedMemory *shm = new QSharedMemory("shm_rt",0);
    char c;
    if (!shm->attach())
    {
        qWarning()<<"Memory attachment failed";
        return 0;
    }

    SharedControl* ptrShm = (SharedControl *) shm->data();

    if (argc==1)
    {
        qWarning()<<"no valid parameter.";
        return 0;
    }

    QTextStream out(stdout);
    while ((c = getopt (argc, argv, "c:s:h:p:q")) != -1)
    {
        switch (c)
        {
        case 'c':
            ptrShm->fpgaSel = atoi(optarg)/2;
            ptrShm->chanSel = atoi(optarg)%2;
            //qWarning()<<"fpgaSel: "<<ptrShm->fpgaSel;
            //qWarning()<<"chanSel: "<<ptrShm->chanSel;
            break;
        case 's':
            if (atoi(optarg)==1)
                ptrShm->avrgSpec = AV_1;
            else if (atoi(optarg)==8)
                ptrShm->avrgSpec = AV_8;
            else if (atoi(optarg)==16)
                ptrShm->avrgSpec = AV_16;
            else if (atoi(optarg)==32)
                ptrShm->avrgSpec = AV_32;
            //qWarning()<<"spectrum Avrg: "<<ptrShm->avrgSpec;
            break;
        case 'h':
            if (atoi(optarg)==1)
                ptrShm->avrgHisto = AV_1;
            else if (atoi(optarg)==8)
                ptrShm->avrgHisto = AV_8;
            else if (atoi(optarg)==16)
                ptrShm->avrgHisto = AV_16;
            else if (atoi(optarg)==32)
                ptrShm->avrgHisto = AV_32;
            //qWarning()<<"histogram Avrg: "<<ptrShm->avrgHisto;
            break;
        case 'q':
            out<<"<c>"+QString::number((ptrShm->chanSel)+(ptrShm->fpgaSel)*2)+"</c>"<<endl;
            out<<"<s>"+QString::number(ptrShm->avrgSpec)+"</s>"<<endl;
            out<<"<h>"+QString::number(ptrShm->avrgHisto)+"</h>"<<endl;
            out<<"<p>"+QString::number(ptrShm->fftPoints)+"</p>"<<endl;
            break;
        case 'p':
            ptrShm->fftPoints=(int) log2(32768/atoi(optarg));
        }
    }
    shm->detach();
    delete shm;
    return 0;
}
