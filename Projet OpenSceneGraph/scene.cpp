/* GOSEZ Alexis -------------------------------------------------------------- 11/2015
	Création d'une scène contenant un terrain, des objets et des différentes lumières
*/


// Inclusion pour gérer la scène
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/StateSet>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/Matrix>
#include <osg/Geometry>
#include <iostream>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/Fog>

#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>


// Céation d'un callback qui permet la rotation
class rotationtCallback : public osg::NodeCallback {                            
public:
    rotationtCallback() : _angle(0.){}
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

protected:
    double _angle;

};


// Méthode de rotation
void rotationtCallback::operator()(osg::Node* node, osg::NodeVisitor* nv){
    osg::MatrixTransform* mT=dynamic_cast<osg::MatrixTransform*>(node);
    osg::Matrix mRotation;

     mRotation.makeRotate(_angle, osg::Vec3(0.,1.,0.));

    mT->setMatrix(mRotation);

    _angle+=0.01;

    traverse(node,nv);
}


int main()
{
/* Création des objets */

	// Création du Viewer
	osgViewer::Viewer viewer ;

	// Création du noeud racine
	osg::ref_ptr<osg::Group> root (new osg::Group);

	// Une géode
   	osg::ref_ptr<osg::Geode> myshapegeode (new osg::Geode);

	/*CESSNA 1*/
	// Création d'un MatrixTransform mère
	osg::ref_ptr<osg::MatrixTransform> cessnaMATmere (new osg::MatrixTransform);        
    osg::ref_ptr<osg::MatrixTransform> cessnaMAT (new osg::MatrixTransform);

	// node contenant un premier Cessna
	// chargement de l'objet 3D
    osg::ref_ptr<osg::Node> cessnanode (osgDB::readNodeFile("cessna.osg"));             
    cessnaMATmere->addChild(cessnaMAT.get());
    cessnaMAT->addChild(cessnanode.get());

    /*CESSNA 2*/
	// Matrix Transform pour le deuxième Cessna
    osg::ref_ptr<osg::MatrixTransform> cessnaMAT2 (new osg::MatrixTransform);
    osg::Matrix cessnaScaleMatrix2;
    cessnaScaleMatrix2.makeScale(1,-1,1);

	// node associé au deuxième cessna
    osg::ref_ptr<osg::Node> cessnanode2 (osgDB::readNodeFile("cessna.osg"));            

    cessnaMAT2->addChild(cessnanode2.get());
    cessnaMAT2->setMatrix(cessnaScaleMatrix2);


    /*CESSNA 3*/
    osg::ref_ptr<osg::MatrixTransform> cessnaMAT3 (new osg::MatrixTransform);

	// node associé au troisième cessna
    osg::ref_ptr<osg::Node> cessnanode3 (osgDB::readNodeFile("cessna.osg"));            

    cessnaMATmere->addChild(cessnaMAT3.get());
    cessnaMAT3->addChild(cessnanode3.get());


    /*POSITION CESSNA 2*/

    osg::ref_ptr<osg::PositionAttitudeTransform> posCessna2 (new osg::PositionAttitudeTransform);
    posCessna2->addChild(cessnaMAT2.get());
    posCessna2->setPosition(osg::Vec3f (0,-500,100));


    /*CALLBACK DE TRANSLATION*/

	//On met en place un callback pour effectuer les translations des cessna dans la scène
    osg::ref_ptr<osg::AnimationPath> path (new osg::AnimationPath);
	path->setLoopMode(osg::AnimationPath::NO_LOOPING);

	osg::ref_ptr<osg::AnimationPath> path2 (new osg::AnimationPath);
	path2->setLoopMode(osg::AnimationPath::NO_LOOPING);

	osg::ref_ptr<osg::AnimationPath> path3 (new osg::AnimationPath);
	path->setLoopMode(osg::AnimationPath::NO_LOOPING);

	// Coordonnées pour les déplacements lors des translations
	osg::AnimationPath::ControlPoint pointA(osg::Vec3(0,500,500));
	osg::AnimationPath::ControlPoint pointB(osg::Vec3(0,-800,100));
	osg::AnimationPath::ControlPoint pointC(osg::Vec3(0,-500,100));
	osg::AnimationPath::ControlPoint pointD(osg::Vec3(0,800,100));
	osg::AnimationPath::ControlPoint pointE(osg::Vec3(0,-800,900));
	
	// On définit le chemin à partir des points créés
	path->insert(0.0f, pointA);
	path->insert(15.0f, pointB);

	path2->insert(0.0f, pointC);
	path2->insert(15.0f, pointD);

	path3->insert(0.0f, pointA);
	path3->insert(15.0f, pointE);

	osg::ref_ptr<osg::AnimationPathCallback> cbTranslation (new osg::AnimationPathCallback(path));
	cessnaMATmere->setUpdateCallback(new rotationtCallback);

	// Translation du premier Cessna
    cessnaMAT->setUpdateCallback(cbTranslation);
	
	// Translation pour le deuxième Cessna
	osg::ref_ptr<osg::AnimationPathCallback> cbTranslation2 (new osg::AnimationPathCallback(path2));
	cessnaMAT2->setUpdateCallback(cbTranslation2);

	// Translation pour le troisième Cessna
	osg::ref_ptr<osg::AnimationPathCallback> cbTranslation3 (new osg::AnimationPathCallback(path3));
	cessnaMAT3->setUpdateCallback(cbTranslation3);


	/*TOUR*/

	// On insère une tour objet 3D dans la scène
    osg::ref_ptr<osg::MatrixTransform> tourMAT (new osg::MatrixTransform);
    osg::Matrix tourScaleMatrix;
    tourScaleMatrix.makeScale(20,20,20);

	// Chargement de l'objet
    osg::ref_ptr<osg::Node> tournode (osgDB::readNodeFile("tour.obj"));

    tourMAT->addChild(tournode.get());
    tourMAT->setMatrix(tourScaleMatrix);


    /*POSITION TOUR*/

	// On définit la position à laquelle on charge la tour
    osg::ref_ptr<osg::PositionAttitudeTransform> tourPOS (new osg::PositionAttitudeTransform);
    tourPOS->addChild(tourMAT.get());
    tourPOS->setPosition(osg::Vec3f (250,0,40));



/* TERRAIN */

	// On charge le terrain
    osg::ref_ptr<osg::MatrixTransform> terrainScaleMAT (new osg::MatrixTransform);
    osg::Matrix terrainScaleMatrix;
    terrainScaleMatrix.makeScale(0.5,0.5,0.5);

    osg::ref_ptr<osg::Node> terrainnode (osgDB::readNodeFile("Terrain2.3ds"));

    terrainScaleMAT->addChild(terrainnode.get());
    terrainScaleMAT->setMatrix(terrainScaleMatrix);

/* TEXTURES */
	
	// Texture plaquée sur le terrain
	osg::ref_ptr<osg::StateSet> textureNodeStateSet (terrainnode->getOrCreateStateSet());
    osg::ref_ptr<osg::Image> image (osgDB::readImageFile("Terrain3.tga"));
    osg::ref_ptr<osg::Texture2D> tex (new osg::Texture2D);

    tex->setImage(image);

    textureNodeStateSet->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);


/* LIGHTING */

	// Création d'un groupe contenant les diverses lumières
    osg::ref_ptr<osg::Group> lightGroup (new osg::Group);
    osg::ref_ptr<osg::StateSet> lightSS (root->getOrCreateStateSet());

	// Création de la première lumière
    osg::ref_ptr<osg::LightSource> lightSource1 (new osg::LightSource);

    // lumière bleue
    osg::Vec4f lightPosition(osg::Vec4f(0, 0, 100, 1.0f));
    osg::ref_ptr<osg::Light> mylight (new osg::Light);
    mylight->setLightNum(1);
        mylight->setPosition(lightPosition);
        mylight->setAmbient(osg::Vec4f(0,0,1,1.0));
        mylight->setDiffuse(osg::Vec4f(0,0,1,1.0));
        mylight->setConstantAttenuation(0.1f);
        mylight->setDirection(osg::Vec3f(-1,0,0));

    lightSource1->setLight(mylight);
    lightSource1->setLocalStateSetModes(osg::StateAttribute::ON);
    lightSource1->setStateSetModes(*lightSS,osg::StateAttribute::ON);

    lightGroup->addChild(lightSource1.get());

/* GRAPHE DE SCENE */

	// On relie les différents objets à la racine de la scène
    root->addChild(cessnaMATmere.get());
    root->addChild(posCessna2.get());
    root->addChild(tourPOS.get());
    root->addChild(terrainnode.get());
    root->addChild(lightGroup.get());

	// On initialise les données de la scène
	viewer.setSceneData( root.get() );

/* ENTREE CLAVIER */

 	// Permet de prendre en charge les entrées clavier
	viewer.addEventHandler(new osgViewer::StatsHandler);

	// Prise en compte du redimmensionnement de la fenêtre
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);

	// Pour le déplacement de la caméra
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );


/* LANCEMENT DU VIEWER */

	return (viewer.run());

}
