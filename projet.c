#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include "projet.h"
#include <stdbool.h>


//******************************************************************************************************************
void* _safeMalloc(const char* filename,int line, size_t size)
	{
	void* ptr= malloc(size);
	if(ptr==NULL)
		{
		fprintf(stderr,"[%s:%d] Probleme allocation mémoire",filename,line);
		exit(EXIT_FAILURE);\
		}
	return ptr;
	}
//******************************************************************************************************************

//EN CONSTRUCTION - FONCTION creer_nouveau_dictionnaire



//******************************************************************************************************************


int calcul_position_caractere_suivant(TSequence* p_sequence, TOccurrence* p_occurrence, char** adr_Liste_seq)
{
	
	int num_sequence_current = 0, j=0 ;
	num_sequence_current = p_sequence->num_seq;
	int longueur_sequence = strlen(adr_Liste_seq[num_sequence_current-1]);
	j = p_occurrence->pos + p_occurrence->i - p_occurrence->d ; 
	if (j == longueur_sequence) j= -1;  // si on arrive à la fin de la séquence, j prend la valeur de -1. 

	return j ; 	
}
//******************************************************************************************************************
//******************************************************************************************************************

void ajout_tete_motif (TMotif** adr_tete_motif, char* word)
{
	TMotif* motif_current = safeMalloc(sizeof(TMotif));
	motif_current->motif_next = NULL;
	motif_current->tete_sequence = NULL;
	motif_current->Motif = safeMalloc(sizeof(char));
	strcpy(motif_current->Motif,word);
	motif_current->motif_next = *adr_tete_motif;
	*adr_tete_motif = motif_current;

}
//******************************************************************************************************************

void ajout_tete_sequence (TSequence** adr_tete_sequence, int num_sequence)
{
	TSequence* sequence_current = safeMalloc(sizeof(TSequence));
	sequence_current->seq_next = NULL;
	sequence_current->tete_occurrence = NULL;
	sequence_current->num_seq = num_sequence;
	sequence_current->seq_next = *adr_tete_sequence;
	*adr_tete_sequence = sequence_current;

}

//******************************************************************************************************************

void ajout_tete_occurrence (TOccurrence** adr_tete_occurrence, TOccurrence* new_occurrence)
{

	TOccurrence* occurrence_current = safeMalloc(sizeof(TOccurrence));
	occurrence_current->occ_next = NULL;

	occurrence_current->occ_next = *adr_tete_occurrence;
	*adr_tete_occurrence = occurrence_current;
	

}

//******************************************************************************************************************

void affectation (TMotif** tete_motif, TMotif* prec_motif, TSequence* prec_sequence, TOccurrence* prec_occurrence, TOccurrence* new_occurrence)
{
	if (prec_occurrence != NULL)
	{
		prec_occurrence->occ_next = new_occurrence; 
		return;
	}

	//prec_occurrence == NULL
	if (prec_sequence != NULL)
	{
		prec_sequence->seq_next->tete_occurrence = new_occurrence; 
		return;
	}

	//prec_sequence == NULL
	if(prec_motif != NULL)
	{
		prec_motif->motif_next->tete_sequence->tete_occurrence = new_occurrence; 
		return;
	}
	(*tete_motif)->tete_sequence->tete_occurrence = new_occurrence;
}

//*****************************************************************************************************************
bool existe_motif(TMotif* tete_motif, char* word, TMotif** adr_prec_motif)
{

	TMotif* p = safeMalloc(sizeof(TMotif));
	p= tete_motif;
	*adr_prec_motif = NULL;

	while((p != NULL) && strcmp(p->Motif,word) < 0){
		
		*adr_prec_motif = p;
		 
		p = p->motif_next;

	}

	return ((p != NULL) && (strcmp(p->Motif,word) == 0)); //vrai
}

//*****************************************************************************************************************

bool existe_sequence(TSequence* tete_seq, int num_sequence, TSequence** adr_prec_sequence)
{
	TSequence* p = safeMalloc(sizeof(TSequence));
	p = tete_seq;
	*adr_prec_sequence = NULL;

	while((p != NULL) && (p->num_seq < num_sequence)){
		
		*adr_prec_sequence = p;
		 
		p = p->seq_next;

	}
	
	return ((p != NULL) && (p->num_seq == num_sequence)); //vrai
}

//*****************************************************************************************************************

bool existe_occurrence(TOccurrence* tete_occ, int pos_occurrence, TOccurrence** adr_prec_occurrence)
{

	TOccurrence* p = safeMalloc(sizeof(TOccurrence));
	p = tete_occ;

	*adr_prec_occurrence = NULL;

	while ((p != NULL) && (p->pos < pos_occurrence)){
		*adr_prec_occurrence = p;
		p = p->occ_next;

	}

	return ((p != NULL) && (p->pos == pos_occurrence)); //vrai
}

//*****************************************************************************************************************

bool existe(TMotif* tete_motif, char* word, int num_sequence, int pos_occurrence, bool *adr_presence_motif, bool *adr_presence_sequence, bool *adr_presence_occurrence, TMotif** adr_prec_motif, TSequence** adr_prec_sequence, TOccurrence** adr_prec_occurrence)
{
	TMotif* motif_current = safeMalloc(sizeof(TMotif));
	motif_current = NULL;
	*adr_presence_motif = false;
	*adr_presence_sequence = false;
	
	if(!existe_motif(tete_motif, word, adr_prec_motif))
	{

		return false;
	}

	*adr_presence_motif = true;

	if (*adr_prec_motif == NULL)
	{

		motif_current = tete_motif;
	}
	else{
		motif_current = (*adr_prec_motif)->motif_next;

	}

	if((motif_current != NULL) && existe_sequence(motif_current->tete_sequence, num_sequence, adr_prec_sequence))
	{

		*adr_presence_sequence = true;

		if((*adr_prec_sequence == NULL) && existe_occurrence(motif_current->tete_sequence->tete_occurrence, pos_occurrence, adr_prec_occurrence))
			{return true;}

		if((*adr_prec_sequence != NULL) && (existe_occurrence((*adr_prec_sequence)->seq_next->tete_occurrence, pos_occurrence, adr_prec_occurrence)))
		{	
			
			return true;
		}

	}
	return false;
}

//*****************************************************************************************************************

void creation_motif (TMotif** adr_tete_motif, char* word, TMotif* prec_motif)
{

	if (prec_motif == NULL)
	{
		ajout_tete_motif(adr_tete_motif, word);
	}
	else{
		ajout_tete_motif(&prec_motif->motif_next, word);
	}
}

//*****************************************************************************************************************

void creation_ligne_sequence (TSequence** adr_tete_sequence, int num_sequence, TSequence* prec_sequence)
{
	if (prec_sequence == NULL)
	{
		ajout_tete_sequence(adr_tete_sequence, num_sequence);
	}
	else{
		ajout_tete_sequence(&prec_sequence->seq_next, num_sequence);
	}
}

//*****************************************************************************************************************

void creation_col_occurrence(TOccurrence** adr_tete_occurrence, TOccurrence* new_occurrence, TOccurrence* prec_occurrence)
{

	if (prec_occurrence == NULL)
	{
		ajout_tete_occurrence(adr_tete_occurrence, new_occurrence);

	}
	else{
		ajout_tete_occurrence(&prec_occurrence->occ_next, new_occurrence);
	}
}

//*****************************************************************************************************************

void creer_col_occurrence(TMotif** tete_motif, char* word, int num_sequence, bool presence_motif, bool presence_sequence, TMotif* prec_motif, TSequence* prec_sequence, TOccurrence* prec_occurrence, TOccurrence* new_occurrence)
{

	TMotif* motif_current = safeMalloc(sizeof(TMotif));

	if (!presence_motif)
	{

		creation_motif(tete_motif, word, prec_motif);
		prec_sequence = NULL;
	}

	if (prec_motif == NULL)
	{	
		motif_current = *tete_motif;

	}
	else{

		motif_current = prec_motif->motif_next;
	}

	if (!presence_sequence)
	{
		creation_ligne_sequence(&motif_current->tete_sequence, num_sequence, prec_sequence);
		prec_occurrence = NULL;

	}

	if(prec_sequence == NULL){

		creation_col_occurrence(&motif_current->tete_sequence->tete_occurrence, new_occurrence, prec_occurrence);


	}else{
		creation_col_occurrence(&prec_sequence->seq_next->tete_occurrence, new_occurrence, prec_occurrence);
	}

	affectation(tete_motif, prec_motif, prec_sequence, prec_occurrence, new_occurrence);


}

//*****************************************************************************************************************


void set(TMotif** tete_motif, char* word, int num_sequence, int pos_occurrence, TOccurrence* new_occurrence)
{
	TMotif* prec_motif= safeMalloc(sizeof(TMotif));
	prec_motif = NULL;
	TSequence* prec_sequence=safeMalloc(sizeof(TSequence));
	prec_sequence = NULL;
	TOccurrence* prec_occurrence = safeMalloc(sizeof(TOccurrence));
	prec_occurrence = NULL;
	bool presence_motif;
	bool presence_sequence;
	bool presence_occurrence;

	bool valeur_remplacee_est_val_def = !existe(*tete_motif, word, num_sequence, pos_occurrence, &presence_motif, &presence_sequence, &presence_occurrence, &prec_motif, &prec_sequence, &prec_occurrence);

	if(valeur_remplacee_est_val_def){

		creer_col_occurrence(tete_motif, word, num_sequence, presence_motif, presence_sequence, prec_motif, prec_sequence, prec_occurrence, new_occurrence);
	}
	else{
		affectation(tete_motif, prec_motif, prec_sequence, prec_occurrence, new_occurrence);
	}

}

//*****************************************************************************************************************


//fonction permettant de compter le nombre de séquences du fichier donné en entrée. 

int compte_nb_sequences(char* filename_seq) 
{
	FILE* file_seq=NULL; 
	int nombre_sequences=0;
	char seq[FILENAME_MAX];  

	if (filename_seq != NULL) 
	{
		file_seq=fopen(filename_seq,"r");
	}	

	if ( file_seq == NULL) 
	{ 
		fprintf(stderr, "Cannot open file \n");	
	}
	
	while(fgets(seq,FILENAME_MAX, file_seq ) != NULL) 
	{
		if((seq[0] == '>') || (seq[0] == '\n'))continue;  // ne compte pas les noms des séquences et les lignes vides. 
		nombre_sequences++;
		
	}

	fprintf(stderr, "le nombre de séquence est de %d\n", nombre_sequences);

	fclose(file_seq);

	return nombre_sequences;
}


//*****************************************************************************************************************

void OpenFile(char* filename_seq, char** Liste_seq) 
{
	FILE* file_seq=NULL; 
	char seq[FILENAME_MAX];
	int cpt_seq =0 ;

	if (filename_seq != NULL) 
	{
		file_seq=fopen(filename_seq,"r");
	}	

	if ( file_seq == NULL) 
	{
			fprintf(stderr, "Cannot open file \n");	
	}

	while(fgets(seq,FILENAME_MAX, file_seq ) != NULL) // lecture du fichier ligne par ligne. 
   	{ 	 
		size_t line_len=strlen(seq);
		if(line_len==0) continue;	// ne prend pas en compte les lignes vides
		if((seq[0] == '>') || (seq[0] == '\n'))continue;  // ne prend pas en compte les sauts à la ligne et les lignes avec le nom des séquences. 
		if(seq[line_len - 1] == '\n') { seq[line_len - 1] = '\0';}   // transforme le saut à la ligne en \0.	
		else { fprintf(stderr,"Cannot read file [%s]\n",strerror(errno));}
		
		Liste_seq[cpt_seq] = (char*)malloc(sizeof(char)*(line_len+1)); // allocation de la mémoire pour la nouvelle séquence
		strcpy(Liste_seq[cpt_seq], seq);  // stockage de la nouvelle séquence dans Liste_seq. 
		fprintf(stderr, "*****\n");
		fprintf(stderr, "%s\n", Liste_seq[cpt_seq]);
		cpt_seq++;	

    	}

	fclose(file_seq);	

}
//*****************************************************************************************************************

//calcul du minimum entre deux entiers : 
#define min(a,b) (a<=b?a:b)


//******************************************************************************************************************

char* conversion_car_chaine(char caractere)
{
	char* stock_nucleotide = safeMalloc(sizeof(char)*2);

	if (stock_nucleotide == NULL)
	{
		fprintf(stderr, "erreur malloc dans la fonction conversion_car_chaine");
	}
	else{

		stock_nucleotide[0] = caractere;
	}

	return stock_nucleotide;
}


//*****************************************************************************************************************
void creer_dictionnaire__motif_vide(TMotif** tete_motif, char* word, int nombre_sequences, char** Liste_seq, int d_Levenshtein)
{
	int i =0, nb_max_insertions = 0;
	for (i=0; i < nombre_sequences; i++)
	{
		int len = strlen(Liste_seq[i]);
		int j = 0;
		for (j=0; j<len;j++)
		{
			TOccurrence* occurrence_current = safeMalloc(sizeof(TOccurrence));
			occurrence_current->pos=j+1;
			occurrence_current->seq=i+1;
			occurrence_current->i=0;
			occurrence_current->s=0;
			occurrence_current->d=0;
			occurrence_current->last="m";
			occurrence_current->occ_next = NULL;
			set(tete_motif, word, i+1, j+1, occurrence_current);
			
			nb_max_insertions = min(d_Levenshtein, (len-j));
			int r=1;
			/*for (r=1; r<nb_max_insertions; r++) { 
				TOccurrence* occurrence_insertion = safeMalloc(sizeof(TOccurrence));
				occurrence_insertion->pos=j+1;
				occurrence_insertion->seq=i+1;
				occurrence_insertion->i=r;
				occurrence_insertion->s=0;
				occurrence_insertion->d=0;
				occurrence_insertion->last="i";
				occurrence_insertion->occ_next = NULL;
				char* newmotif = safeMalloc(sizeof(char)*2);
				char* new_nucleotide = safeMalloc(sizeof(char));
				new_nucleotide = conversion_car_chaine(Liste_seq[i][j]);
				strcat(newmotif, new_nucleotide);
				fprintf(stderr, "motif : %s\n",newmotif); 
				set(tete_motif, newmotif, i+1, j+1, occurrence_insertion);
			}*/

		}
	}

}

//******************************************************************************************************************
int main(int argc, char** argv)
{

	int c=0, option_index = 0;
	char* filename_seq = safeMalloc(sizeof(char));
	filename_seq = NULL;
	int long_motif, d_Levenshtein;
	float quorum;
	int nombre_sequences;


        static struct option long_options[] = {
       	    {"help", no_argument, NULL, 'h'},
            {"filename_seq",  required_argument, 0, 'f'},
            {"longueur_motif",  required_argument, 0, 'm'},
            {"d_Levenshtein",  required_argument, 0, 'd'},
            {"quorum",  required_argument, 0, 'q'},
            {0,		0,	  0,	 0 }
        };
        
	while ((c = getopt_long(argc, argv,"hf:m:d:q:", long_options, &option_index)) != -1) {

		switch(c) {

			case 'h': /*usage();return EXIT_SUCCESS; */  break;
			
			case 'f': filename_seq = optarg; break;
			 
			case 'm': long_motif = atoi(optarg); break;
			
			case 'd': d_Levenshtein = atoi(optarg); break;

			case 'q': quorum = atoi(optarg); break;
			 
			case '?': fprintf(stderr, "ERROR: option -%c is undefined\n", optopt); return EXIT_FAILURE; break;
			
			case ':': fprintf(stderr, "ERROR: option -%c requires an argument\n",optopt); return EXIT_FAILURE; break;
			 	
			default : /* usage(); return EXIT_FAILURE; */break;
	
		}
	}

	nombre_sequences = compte_nb_sequences(filename_seq);
	char* Liste_seq[nombre_sequences-1];
	char** p_Liste_seq = &Liste_seq[0];
	OpenFile(filename_seq, p_Liste_seq);

	int i;
	for (i=0; i < nombre_sequences; i++)
	{
	fprintf(stderr, "liste_dans_main: ");
	fprintf(stderr, "%s \n", Liste_seq[i]);	
	}



//********************************************************
	// Dictionnaire vide
	

	TMotif* motif_vide = safeMalloc(sizeof(TMotif));
	motif_vide = NULL;
	
	creer_dictionnaire__motif_vide(&motif_vide, "", nombre_sequences, Liste_seq, d_Levenshtein);
	
//*********************************************************
	//fprintf(stderr, "nombre d'occurrence %s\n", motif_vide->Motif); 

	TMotif* new_tete_motif = safeMalloc(sizeof(TMotif));
	new_tete_motif = motif_vide;

	//début algo 3
	for (i = 0; i <= long_motif-1; ++i)
	{
		TMotif* p_motif = safeMalloc(sizeof(TMotif));
		p_motif = new_tete_motif;
		//creation dictionnaire de taille i:
		TMotif* tete_new_dict = safeMalloc(sizeof(TMotif));
		tete_new_dict = NULL;
		while (p_motif != NULL) //pour chaque motif M
		{
			//fprintf(stderr, "nb seq avec occurrence: %d\n", p_motif->nb_seq_avec_occurrence);
			TSequence* p_sequence = safeMalloc(sizeof(TSequence));
			p_sequence = p_motif->tete_sequence;
			
			while (p_sequence != NULL) //pour chaque sequence
			{
				fprintf(stderr, "num sequence: %d\n", p_sequence->num_seq);
				TOccurrence* p_occurrence = safeMalloc(sizeof(TOccurrence));
				p_occurrence = p_sequence->tete_occurrence;

				while (p_occurrence != NULL) //pour chaque occurrence
				{	
					
					int j = calcul_position_caractere_suivant(p_sequence, p_occurrence, p_Liste_seq); //calcul de la position j du caractère suivant
					
					fprintf(stderr, "%d\n",j);
					fprintf(stderr, "mot: %s\n", p_motif->Motif);
					fprintf(stderr, "pos occurrence: %d\n", p_occurrence->pos);
					
					// match ?
					bool match_possible = true;
					if (j == -1) {
						match_possible = false;
						//continue; 
					}

					if ((strcmp(p_occurrence->last,"insertion")==0) && (p_Liste_seq[p_sequence->num_seq-1][j] == p_Liste_seq[p_sequence->num_seq-1][j-1])) continue;

					fprintf(stderr, "%c\n", p_Liste_seq[p_sequence->num_seq-1][j-1]);
					
					// il faudrait concater l'ancien motif avec la suite j+1
					//début de l'élongation + ajout
					char* newmotif = safeMalloc(sizeof(char));
					newmotif = NULL;
					
					if (strcmp(p_motif->Motif, "") == 0)
					{
						newmotif = safeMalloc(sizeof(char)*2);

					}
					else
					{
						newmotif = p_motif->Motif ;
					}


					char* new_nucleotide = safeMalloc(sizeof(char));
					new_nucleotide = conversion_car_chaine(p_Liste_seq[p_sequence->num_seq-1][j]);
					strcat(newmotif, new_nucleotide);
					fprintf(stderr, "motif : %s\n",newmotif);

					// ICI fonction set
					
					//fin de l'élongation + ajout


					p_occurrence = p_occurrence->occ_next;

				}


				p_sequence = p_sequence->seq_next;
				fprintf(stderr, "%p\n", p_sequence);
			}


			p_motif = p_motif->motif_next;
		}
		

		//free
	
		TMotif* tmp = p_motif;
		TMotif* tmpmotifnext;
		TSequence* tmpsequencenext;
		TOccurrence* tmpoccurrencenext;

		while(tmp != NULL){
			while(tmp->tete_sequence != NULL){
				while(tmp->tete_sequence->tete_occurrence != NULL) {
					tmpoccurrencenext = tmp->tete_sequence->tete_occurrence->occ_next;
					free(tmp->tete_sequence->tete_occurrence);
					tmp->tete_sequence->tete_occurrence=tmpoccurrencenext;
				}	
				tmpsequencenext = tmp->tete_sequence->seq_next;
				free(tmp->tete_sequence);
				tmp->tete_sequence=tmpsequencenext;
			}
			tmpmotifnext = tmp->motif_next;
			free(tmp);
			tmp=tmpmotifnext;
		}
	
		free(p_motif);
		free(tmp);

		//ici détruire le dictionnaire de taille i-1
		// et écrire "new_tete_motif = tete_new_dict"
	}
	//fin algo 3

}
