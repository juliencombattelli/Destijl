#include "imagerie.h"

using namespace cv;
using namespace raspicam;
using namespace std;

float calculAngle(position * positionRobot); // fonction privée
int cropArena(Image *imgInput, Image *imgOutput, Arene *AreneInput); // fonction privée
float euclideanDist(Point& p, Point& q); // fonction privée

void drawArena(Image *imgInput, Image *imgOutput, Arene *monArene)
{
    if(imgInput!=imgOutput)
        *imgOutput=imgInput->clone();
    rectangle(*imgOutput,monArene->tl(),monArene->br(),Scalar(0,0,125),2,8,0);
}


int openCamera(RaspiCam_Cv  *Camera)
{
    Camera->set(CV_CAP_PROP_FORMAT, CV_8UC3);
    Camera->set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);
    Camera->set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT);

    printf("Opening Camera...\n");
    if (!(Camera->open())) {
        perror("Can't open Camera\n") ;return -1;
    }
    else
    {
        printf("Camera warmup 2sec\n");
        sleep(2);
        printf("Start capture\n");
        return 0;
    }
}

void getImg(RaspiCam_Cv *Camera, Image * monImage) // getImg(Camera, Image img);
{
    Camera->grab();
    Camera->retrieve(*monImage);
    cvtColor(*monImage,*monImage,CV_BGR2RGB);
}

void closeCam(Camera *Camera) // closeCam(Camera) : camera Entrer
{
    Camera->release();
}


int detectArena(Image *monImage, Arene *rectangle) // Image en entrée // rectangle en sortie
{
    vector<vector<Point> > contours;
    vector<Point> approx;
    vector<Vec4i> hierarchy;

    Image imageTrt;
    cvtColor(*monImage,imageTrt,CV_RGB2GRAY); // conversion en niveau de gris
    threshold(imageTrt,imageTrt,128,255,CV_THRESH_BINARY); // Threshold les éléments les plus clair
    Canny(imageTrt, imageTrt, 100,200,3); // detection d'angle

    findContours(imageTrt, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    for(unsigned int i = 0; i < contours.size();i++)
	{
        approxPolyDP(Image(contours[i]), approx, arcLength(Image(contours[i]), true)*0.1, true);
        if(approx.size()==4 && fabs(cv::contourArea(contours[i])) > 100000)
        {
            *rectangle = boundingRect(Image(contours[i]));
            return 0;
        }
    }
    return -1;
}

int cropArena(Image *imgInput, Image *imgOutput, Arene *AreneInput) // image // rectangle // image2
{
    Image img;
    img=imgInput->clone();

    *imgOutput = img(*AreneInput);
    return 0;
}

float euclideanDist(Point& p, Point& q) {
    Point diff = p - q;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}

void imgCompress(Image *imgInput, Jpg *imageCompress) // image entrée // imageEncodé en sortie
{
    imencode(".jpg",*imgInput,*imageCompress);
}


int detectPosition(Image *imgInput, position *posTriangle, Arene * monArene) // entree : image  / sortie tab pos
{
    vector<vector<Point> > contours;
    vector<Point> approx;
    vector<Vec4i> hierarchy;

    Image imgTraitment;

    if(monArene==NULL)
        imgTraitment=imgInput->clone();
    else
        cropArena(imgInput,&imgTraitment, monArene);

    cvtColor(imgTraitment,imgTraitment,CV_RGB2GRAY);
    threshold(imgTraitment,imgTraitment,128,255,CV_THRESH_BINARY);
    findContours(imgTraitment, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    int nbrTriangle = 0;
    for(unsigned int i = 0;i < contours.size();i++)
	{
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.17, true);
        if(approx.size() == 3 && fabs(contourArea(contours[i])) > 200 && fabs(contourArea(contours[i])) < 700)
        {

            Point a,b,c;
            Point center;


            a = approx[0];
            b = approx[1];
            c = approx[2];


            if(monArene !=NULL) // ajout de l'offset de l'arène
            {
               a.x += monArene->x;
               a.y += monArene->y;
               b.x += monArene->x;
               b.y += monArene->y;
               c.x += monArene->x;
               c.y += monArene->y;		    
            }
		
            center.x = (a.x + b.x + c.x)/3;
            center.y = (a.y + b.y + c.y)/3;

            posTriangle[nbrTriangle].center=center;

            if(euclideanDist(center,b) > euclideanDist(center,a) && euclideanDist(center,b) > euclideanDist(center,c) )
            {

                posTriangle[nbrTriangle].direction=b;
                //line(img,center,b,Scalar(0,125,0),2,8,0);
            }
            else if(euclideanDist(center,a) > euclideanDist(center,c))
            {
                posTriangle[nbrTriangle].direction=a;
                //line(img,center,a,Scalar(0,125,0),2,8,0);

            }
            else
            {
                posTriangle[nbrTriangle].direction=c;
                //line(img,center,c,Scalar(0,125,0),2,8,0);
            }
            posTriangle[nbrTriangle].angle=calculAngle(&posTriangle[nbrTriangle]);

            nbrTriangle++;

        }
    }
    return nbrTriangle;
}



void drawPosition(Image *imgInput, Image *imgOutput, position *positionRobot) // img E/S   pos : E
{
    if(imgInput!=imgOutput)
    {
        *imgOutput=imgInput->clone();
    }
    line(*imgOutput,positionRobot->center,positionRobot->direction,Scalar(0,125,0),2,8,0);
}

float calculAngle(position * positionRobot) // position en entree
{
    float a = positionRobot->direction.x - positionRobot->center.x;
    float b = positionRobot->direction.y - positionRobot->center.y ;
    float angle = atan2(b,a);
    return (angle*180)/3.14;

}
