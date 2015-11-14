#include <stdio.h>
#include <stdlib.h>

typedef struct TOccurrence TOccurrence;
struct TOccurrence  {
	int pos;
	int seq;
	int i;
	int s;
	int d;
	char* last;
	struct TOccurrence *occ_next;
};


typedef struct TSequence TSequence;
struct TSequence{
	int num_seq;
	struct TSequence *seq_next;
	struct TOccurrence *tete_occurrence;
} ;


typedef struct TMotif TMotif;
struct TMotif{
	char* Motif;
	int nb_seq_avec_occurrence;
	struct TSequence *tete_sequence;
	struct TMotif *motif_next;
} ;


#define safeMalloc(S) _safeMalloc(__FILE__,__LINE__,S)
