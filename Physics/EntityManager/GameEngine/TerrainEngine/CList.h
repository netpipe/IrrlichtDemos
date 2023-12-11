/***************************************************************
*                Terrain Engine issue of					   *
*							Dreamotion3D engine				   *
*															   *
**************************************************************** 
*File Created: 06.12.06                                        *
*Author:	Jean-Chritophe Lecomte & TMyke     		           *
*								v 1.0.1						   *
*Contens: CList header class                                   *
***************************************************************/
#ifndef _CLIST_H
#define _CLIST_H

/**
* list class .
* Class allowing a simplified management of list.
* @param none.
* @return none.
*/
template<class type>
class CList														
{

private:

	typedef struct stockpoint {	type * pointeur; };				// Structure encapsulating an element of the list
	inline void mem_Alloc(void);								// alloc memory, for memorize data

	bool isFreeAll;												// flag when call freeAll


public:

	stockpoint * mzone;											/*!Pointer on the zone memory storing the list. 
																	This member is public to allow the derived classes 
																	to reach the elements (friend impossible?).
																	DON'T TO MODIFY THIS VALUE! . */

	int nbrelement;												/*! For a direct reading without passing by count ()
																    DON'T TO MODIFY THIS VALUE !.*/


	//----------------
	// METHODS
	//----------------
	/**
	* Constructor .
	* constructor class.
	* @param none.
	* @return none.
	*/	
	CList(void);											
	/**
	* Destructor .
	* destructor class.
	* @param none.
	* @return none.
	*/	
	~CList(void);											

	/**
	* destroy() .
	* delete the list itself.
	* @param none.
	* @return none.
	*/	
	inline  void destroy(void);									

	/**
	* freeAll() .
	* delete all object in the list (free ressource).
	* and delete the list itself.
	* @param none.
	* @return none.
	*/	
	inline   void freeAll();										

	/**
	* expand() .
	* Add N elements at the end of the list without specifying the contents.
	* @param nbr number of elements.
	* @return none.
	*/	
	inline  void expand(const int& nbr);							
		
	/**
	* listcopy() .
	* Add list elments of 'liste' class in the end of list.
	* @param liste2 list which one will take the elements..
	* @return none.
	*/	
	inline  void listcopy(CList<type> * liste2);				

	/**
	* add() .
	* Add 'pThis' in the end of list.
	* @param pthis one element.
	* @return none.
	*/	
	inline  void add(type * pthis);								

	/**
	* add() .
	* Add 'pthis' if this is not already in the list.
	* @param pthis one element.
	* @return none.
	*/	
	inline  void addunique(type * pthis);							

	/**
	* put() .
	* place 'pthis' at the index position.
	* @param index index place for element.
	* @param pthis one element.
	* @return none.
	*/	
	inline  void put(const int& index,type * pthis);	

	/**
	* find() .
	* find one element in the list, return true if find, with index list.
	* @param pthis one element.
	* @param index index in the list elements.
	* @return result True if pThis is present.
	*/	
	inline  bool find(type * pthis,int &index);					

	/**
	* find() .
	* find one element in the list, return true if find.
	* @param pthis one element.
	* @return result True if pThis is present.
	*/	
	inline  bool find(type * pthis);								
	
	/**
	* find() .
	* find one element in the list, between 'start' ans 'end' index in the list
	* @param pthis one element.
	* @return result True if pThis is present.
	*/	
	inline  bool find(type * pthis, int start, int end);			


	/**
	* del() .
	* Remove the first element containing this (the object does not remove, only its pointer in the list…)
	* @param pthis one element.
	* @return none.
	*/	
	inline  void del(type * pthis);				
	
	/**
	* del() .
	* Remove the element in the 'index' position (the object does not remove, only its pointer in the list…)
	* @param index position in the list.
	* @return none.
	*/	
	inline  void del(const int& index);
	
	/**
	* limite() .
	* Limit the list with a number of elements. (the object does not remove, only its pointer in the list…)
	* @param nbrelement number of elements.
	* @return none.
	*/	
	inline  void limite(int nbrelement);							// Limite la liste à nbrelement ( le solde est supprimé - meme remarque)

	/**
	* get() .
	* return the 'indexed' element
	* @param index position in the list.
	* @return type indexed element in the list.
	*/	
	inline   type * get(const int& index);
	
	/**
	* count() .
	* return the number of elements in the list
	* @param none.
	* @return type indexed element in the list.
	*/	
	inline  int count(void);

	/**
	* flush() .
	* free the list. Don't delete objects.
	* @param none.
	* @return none.
	*/	
	inline  void flush(void);

	/**
	* clean() .
	* erase the elements in the list. and delete objects. Don't delete the list .
	* @param none.
	* @return none.
	*/	
	inline  void clean(void);									

	/**
	* swap() .
	* swa ptwo elements in the list.
	* @param a.
	* @param b.
	* @return none.
	*/	
	inline  void swap(int a,int b);
};




// Constructeur
template<class type>
 CList<type>::CList(void)
{	
	isFreeAll = false;
	nbrelement = 0;
	mzone = NULL;
}

// Destructeur
template<class type>
 CList<type>::~CList(void)
{
	free(mzone);
}

// Vide la liste ET SUPPRIME LES OBJETS - Ne supprime PAS la CList elle même
template<class type>
void CList<type>::clean()
{
	register int i;
	for (i=0;i<nbrelement;i++)
		delete get(i);
	this->flush();
}

// Destruction de la liste - 
template<class type>
void CList<type>::destroy(void)
{
	nbrelement = 0;
	free(mzone);
	mzone = NULL;
	//delete this;
}

// Liberation des ressources - Supprime LES OBJETS pointés par la CList AINSI que la CList en elle meme
template<class type>
void CList<type>::freeAll()
{
	register int i;
	isFreeAll=true;
	for (i=0;i<nbrelement;i++) delete get(i);
	delete this;
}

// Vide la liste de tous ses elements - NE SUPPRIME PAS LES OBJETS
template<class type>
void CList<type>::flush(void)
{
	nbrelement = 0;
	free(mzone);
	mzone = NULL;
}

// Allocation memoire
template<class type>
void CList<type>::mem_Alloc(void)
{
	mzone = (stockpoint*)realloc(mzone,nbrelement<<2);		// stockpoint est sensé faire 4 octets...

	// si mzone est NULL -> erreur d'allocation...
}

// Ajoute n elements en fin de liste sans préciser leur contenu
template<class type>
void CList<type>::expand(const int& nbr)
{
	nbrelement+=nbr;
	mem_Alloc();
}

// Ajoute les éléments de la CList liste en fin de liste
template<class type>
void CList<type>::listcopy(CList<type> * liste)
{
	if (liste->nbrelement==0)
		return;

	int last = nbrelement;
	insert(liste->nbrelement);
	memcpy(&mzone[last], liste->mzone, liste->nbrelement<<2);

}

// Ajoute ceci dans la liste
template<class type>
void CList<type>::add(type * ceci)
{
	nbrelement++;
	mem_Alloc();
	mzone[nbrelement-1].pointeur = ceci;
}

// Ajoute ceci si ceci n'est pas deja dans la liste
template<class type>
void CList<type>::addunique(type * ceci)
{
	if (!find(ceci)) add(ceci);
}


// Place ceci à la nieme position dans la liste
template<class type>
void CList<type>::put(const int& index,type * ceci)
{
	mzone[index].pointeur = ceci; 
}

// Retourne le nombre d'elements dans la liste
template<class type>
int CList<type>::count(void)
{
	return nbrelement;
}

// Retourne le nieme element de la liste
template<class type>
type * CList<type>::get(const int& index)
{
	if ( nbrelement > index ) 
		return mzone[index].pointeur;
	else
		return NULL;
}

// Retourne true si ceci est présent dans la liste et place son index dans &index - PREMIERE OCCURANCE TROUVEE
template<class type>
bool CList<type>::find(type * ceci,int &index)
{
	if (nbrelement==0)
		return false;

	register int count=0;
	register stockpoint * access;
	
	for (access=mzone;access<mzone+nbrelement;access++)
	{
		if (access->pointeur == ceci )
		{
			index = count;
			return true;
		}
		count++;
	}
	return false;
}

// Retourne true si ceci est présent dans la liste
template<class type>
bool CList<type>::find(type * ceci)
{
	if (nbrelement==0)
		return false;

	register stockpoint * access;
	
	for (access=mzone;access<mzone+nbrelement;access++)
		if (access->pointeur == ceci )
			return true;

	return false;
}

// Retourne true si ceci est présent dans la liste entre l'indice debut et l'indice fin
template<class type>
bool CList<type>::find(type * ceci, int debut, int fin)
{
	if (nbrelement==0)
		return false;

	register stockpoint * access;
	
	for (access=mzone+debut;access<mzone+fin;access++)
		if (access->pointeur == ceci )
			return true;

	return false;

}


// Supprime le premier element de la liste contenant ceci
template<class type>
void CList<type>::del(type * ceci)
{
	if (isFreeAll)	return;

	int index;
	if ( find(ceci,index) )
		del(index);
}

// Supprime le nieme element de la liste (base0)
template<class type>
void CList<type>::del(const int& index)
{
	if (isFreeAll)	return;

	if (nbrelement<2)
	{
		nbrelement=0;
		free(mzone);
		mzone=NULL;
		return;
	}

	if (index<nbrelement-1)
		memmove(&mzone[index],&mzone[index+1], (nbrelement-(index+1))*4 );

	--nbrelement;

	mem_Alloc();


}

// Limite la liste à nbrelement ( le solde est supprimé )

template<class type>
void CList<type>::limite(int nbrelement)
{
	this->nbrelement=nbrelement;
	mem_Alloc();
}

// Permute l'emplacement des elements d'indices a et b dans la liste
template<class type>
void CList<type>::swap(int a,int b)
{
	register type * tampon;

	tampon = (mzone+a)->pointeur;

	(mzone+a)->pointeur = (mzone+b)->pointeur;
	(mzone+b)->pointeur = tampon;
}


#endif