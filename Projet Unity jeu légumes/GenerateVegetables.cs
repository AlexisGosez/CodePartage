using UnityEngine;
using System.Collections;

//Pour l'utilisation de List<T>
using System.Collections.Generic;

public class GenerateVegetables : MonoBehaviour
{
	// Légumes pouvant être générés
	public List<GameObject> vegetablesPrefabs;

	// Initialisation
	void Start()
	{
		GenerateVegetable();
	}
	
	// Lorsque l'on appuie sur espace, un légume apparaît
	void Update()
	{
		if(Input.GetKeyDown(KeyCode.Space))
		{
			GenerateVegetable();
		}


		
	}

    // Fonction permettant de générer le légume
	private void GenerateVegetable()
	{
		// On prend un légume u hasard dans la liste
        int randomVegetable = Random.Range(0, vegetablesPrefabs.Count);

        // On le génère
		GameObject newVegetable = GameObject.Instantiate(vegetablesPrefabs[randomVegetable]) as GameObject;

        // On définit la position souhaitée
		float posZ = Random.Range (-3, 4);
		float posX = Random.Range (-3, 4);
		Vector3 positionDepart = new Vector3(posX,4.5f,posZ);


		newVegetable.transform.parent = this.transform;
		newVegetable.transform.localPosition = Vector3.zero;
		newVegetable.transform.localRotation = Quaternion.identity;

		newVegetable.transform.position = positionDepart;


	}



}
