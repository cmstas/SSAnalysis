#ifndef FITS_H
#define FITS_H

float electron_prob(float x); 
 
float muon_prob(float x); 
 
float btag_prob_part(float x); 

float btag_prob_jets(float x); 
 
float met_prob_30(float x);
 
float met_prob_50(float x);
 
float met_prob_120(float x);
 
float ht_prob_part_200(float x);
 
float ht_prob_part_400(float x);

float ht_prob_jets_200(float x);
 
float ht_prob_jets_400(float x);

float jet_prob_part(float x);

float jet_prob_jets(float x);

#endif
