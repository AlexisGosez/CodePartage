/* GOSEZ Alexis ------------------------------------------------------- 9/12/2015
       Script permettant de déplacer la scène avec la technique grab the air
*/

// On utilise le leap motion
using Leap;
using UnityEngine;
using System.Collections;


public class GrabTheAir : MonoBehaviour {


	public HandController handControl;
	public float coefficient;
	public float speedRotation;

	private HandList handListe;
	private Vector3 oldHandPosition;
	private Vector3 newHandPosition;
	private Controller leapController;


    // Contrôle de la main par le leap motion
	void Start() {
		leapController = handControl.GetLeapController ();	
	}


	void Update () {
        // Lorsqu'une main apparaît à l'écran elle est stockée dans le vecteur
		handListe = leapController.Frame ().Hands;

        // Si le vecteur est non vide on récupère la ou les mains présentes
		if (!handListe.IsEmpty) {
			Hand main = handListe [0];
			Hand main2 = handListe [1];
		

			if (main.GrabStrength > 0.5 && main2.GrabStrength < 0.5){
                //Si la première main est fermée et la deuxième ouverte (au cas où deux mains sont présentes)
                //On récupère la position de la main et on la convertie en vecteur qui peut être traité par unity
				newHandPosition = main.PalmPosition.ToUnityScaled();
				oldHandPosition.y *= -1;
				newHandPosition.y *= -1;

                //On déplace la scène en même temps que la main fermée se déplace
				Vector3 newGameObjectPosition = gameObject.transform.position + coefficient * (newHandPosition - oldHandPosition);
				gameObject.transform.position = newGameObjectPosition;
			}

			if (main.GrabStrength > 0.5 && main2.GrabStrength > 0.5){
                // Si les deux mains sont présentes et fermées, on va créer une rotation de la scène
                // en fonction du mouvement

                // On récupère le vecteur entre les deux mains
				Vector3 distance = main.PalmPosition.ToUnityScaled() - main2.PalmPosition.ToUnityScaled();

                // On ne prend pas en compte l'axe vertical (rotation autour de l'axe y souhaitée)
				distance.y = 0;

                // On prend le vecteur orthogonal
				Vector3 orthogonal = Vector3.Cross(distance,Vector3.up);
				float angle = speedRotation*Vector3.Angle(orthogonal, Vector3.forward);


                // On définit ensuite les sens de rotations
				if (main.IsLeft==true){

					if (main.PalmPosition.ToUnityScaled().z<main2.PalmPosition.ToUnityScaled().z){
						angle *= -1;
					}
				}

				else {

					if (main.PalmPosition.ToUnityScaled().z > main2.PalmPosition.ToUnityScaled().z){
						angle *= -1;
					}
				}
				
                // On effectue la rotation de la scène
				Quaternion rotationAngle = Quaternion.identity;
				rotationAngle.eulerAngles =  new Vector3(0,angle,0);
				gameObject.transform.rotation *= rotationAngle;
			}

			oldHandPosition = main.PalmPosition.ToUnityScaled();
		}

	}
}