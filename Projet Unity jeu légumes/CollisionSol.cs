/* GOSEZ Alexis --------------------------------------------------------- 11/11/2015
Script permettant la suppression d'une pastèque ou d'une tomate qui tombe sur le sol
*/

using UnityEngine;
using System.Collections;

public class CollisionSol : MonoBehaviour {

	void OnCollisionEnter (Collision col)
	{
		if(col.gameObject.tag == "vegetables")
		{
			Destroy(col.gameObject);
		}
	}
}
