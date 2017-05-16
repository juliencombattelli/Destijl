#ifndef IMAGERIE_H
#define IMAGERIE_H
#include <raspicam/raspicam_cv.h>
#include "opencv2/imgproc/imgproc.hpp"
#include <unistd.h>
#include <math.h>

#define WIDTH 640 //1280 1024 640
#define HEIGHT 480 // 960 768 480

using namespace std;
using namespace cv;
using namespace raspicam;

typedef Mat Image;
typedef RaspiCam_Cv Camera;
typedef Rect Arene;
typedef vector<unsigned char> Jpg;
struct  position {
    Point center;
    Point direction;
    float angle;
};

/*
 * def : detectPosition detecte la position des triangles dans une image d'entr�e (imgInput).
 * Il d�coupe �galement l'image en cas d'element de type Arene pass� en param�tre.
 * La modification se fait sur imgOutput. La fonction retourne le nombre de triangle trouv�.
 * La position de chaque triangle est stock� dans un tableau de position pass� par pointeur.
 * La position est relative � la resolution de l'image initial (640*480)
 * Ex : Image monImage; position positionRobot[20];
 *      detectPosition(&monImage, positionRobot);
 */
int detectPosition(Image *imgInput, position *posTriangle, Arene * monArene = NULL);


/*
 * def : getImg capture une image avec la camera pass� en entr�e.
 * La cam�ra doit avoir �tre ouverte. (cf openCamera)
 * Ex : Image monImage;
 *      Camera rpiCam;
 *      openCamera(&rpiCam);
 *      getImg(&rpiCam, &monImage);
 */
void getImg(RaspiCam_Cv *Camera, Image * monImage);



/*
 * def : detectArena : retourne 0 si une arene est detect�e, et -1 si aucune arene n'est trouv�e.
 * Les coordonn�es de l'arene sont retourn�es par pointeur.
 * Ex : Arene monArene;
 *      if(detectArena(&imageA, &monArene)==0)
 *      {
 *          printf("Arene detectée\n");
 *      }
 */
int detectArena(Image *monImage, Arene *rectangle);


/*
 * def : Ferme la camera passé en paramétre;
 * Ex:  closeCam(&rpiCam);
 */
void closeCam(Camera *Camera);


/*
 * def : dessine le plus petit rectangle contenant l'aréne en paramétre
 * Ex : Image areneDessine;
 *      drawArena(&imgA, &areneDessine, &monArene);
 */
void drawArena(Image *imgInput, Image *imgOutput, Arene *monArene);


/*
 * def : compresse une image en entrée(imgInput) au format jpg de façon à ce que celle-ci soit prete à etre
 * envoyé à l'UI. L'image compressé est de type Jpg;
 * Ex : Jpg compress;
 *      imgCompress(&imgA, &compress);
 */
void imgCompress(Image *imgInput, Jpg *imageCompress);


/*
 * Ouvre la camera. Retourne 0 si celle-ci est ouverte, -1 si un erreur survient.
 * Il est necessaire de passer comme paramétre une variable de type Camera.
 * Ex : Camera cam;
 *      openCamera(&cam);
 */
int openCamera(RaspiCam_Cv  *Camera);


/*
 * Dessine une position de triangle sur une image de sortie(imgOutput);
 * Ex : if(detectPosition(&monImage,&monImage, positionRobot)>0)
 *          drawPosition(&imgA,&imgA,&positionRobot[0]);
 */
void drawPosition(Image *imgInput, Image *imgOutput, position *positionRobot);



#endif // IMAGERIE_H
