/* GOSEZ Alexis --------------------------------------------------------- 11/11/2015
Script permettant la suppression d'une pastèque ou d'une tomate qui tombe dans le seau
*/

using UnityEngine;
using System.Collections;

public class CollisionSeau : MonoBehaviour {


		void OnCollisionEnter (Collision col)
		{
			if(col.gameObject.tag == "vegetables" && col.gameObject.transform.position.y<-1)
			{
				Destroy(col.gameObject);
			}
		}

}
