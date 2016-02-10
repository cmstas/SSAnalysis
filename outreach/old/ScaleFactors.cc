#include "ScaleFactors.h"
#include "/home/users/cgeorge/CORE/CMS2.h"
#include <utility>
#include <cmath>

float TagAndProbeScaleFactor(int id, float pt, float eta){
    // haven't measured very-low pt
    if (pt < 10)
    {
        return 1.0;
    }
    const double aeta = fabs(eta);

    if (abs(id)==11) 
    {
        if (aeta > 1.4442 && aeta < 1.566)
        {
           // cout << "[ss::TagAndProbeScaleFactor] WARNING -> eta is in the transition region. returning bogus value 1.0" << endl;
           // cout << Form("[ss::TagAndProbeScaleFactor] id: %d pt: %f eta: %f", id, pt, aeta) << endl; 
            return 0;
        }
        
        if (10 < pt && pt < 15)
        {
            if (0.00   < aeta && aeta < 0.80  ) {return 0.834;}
            if (0.80   < aeta && aeta < 1.4442) {return 0.973;}
            if (1.566  < aeta && aeta < 2.00  ) {return 0.954;}
            if (2.00   < aeta                 ) {return 1.119;}
        }
        else if (15 < pt && pt < 20)
        {
            if (0.00   < aeta && aeta < 0.80  ) {return 0.918;}
            if (0.80   < aeta && aeta < 1.4442) {return 0.906;}
            if (1.566  < aeta && aeta < 2.00  ) {return 0.909;}
            if (2.00   < aeta                 ) {return 0.944;}
        }
        else if (20 < pt && pt < 30)
        {
            if (0.00   < aeta && aeta < 0.80  ) {return 0.954;}
            if (0.80   < aeta && aeta < 1.4442) {return 0.923;}
            if (1.566  < aeta && aeta < 2.00  ) {return 0.921;}
            if (2.00   < aeta                 ) {return 0.993;}
        }
        else if (30 < pt && pt < 40)
        {
            if (0.00   < aeta && aeta < 0.80  ) {return 0.960;}
            if (0.80   < aeta && aeta < 1.4442) {return 0.935;}
            if (1.566  < aeta && aeta < 2.00  ) {return 0.924;}
            if (2.00   < aeta                 ) {return 0.959;}
        }
        else if (40 < pt && pt < 50)
        {
            if (0.00   < aeta && aeta < 0.80  ) {return 0.972;}
            if (0.80   < aeta && aeta < 1.4442) {return 0.955;}
            if (1.566  < aeta && aeta < 2.00  ) {return 0.950;}
            if (2.00   < aeta                 ) {return 0.968;}
        }
        else if (pt > 50)
        {
            if (0.00   < aeta && aeta < 0.80  ) {return 0.969;}
            if (0.80   < aeta && aeta < 1.4442) {return 0.956;}
            if (1.566  < aeta && aeta < 2.00  ) {return 0.995;}
            if (2.00   < aeta                 ) {return 0.969;}
        }

        // if we get here, return bogus value
        cout << "[ss::TagAndProbeScaleFactor] WARNING -> missed coverage -- returning bogus value 1.0" << endl;
        cout << Form("[ss::TagAndProbeScaleFactor] id: %d pt: %f eta: %f", id, pt, aeta) << endl; 
        return 0;
    }

    if (abs(id)==13) 
    {
        if (10 < pt && pt < 15)
        {
            if (0.00  < aeta && aeta < 1.20) {return 0.973;}
            if (1.20  < aeta               ) {return 0.954;}
        }
        else if (15 < pt && pt < 20)
        {
            if (0.00  < aeta && aeta < 1.20) {return 0.957;}
            if (1.20  < aeta               ) {return 0.971;}
        }
        else if (20 < pt && pt < 30)
        {
            if (0.00  < aeta && aeta < 1.20) {return 0.964;}
            if (1.20  < aeta               ) {return 0.981;}
        }
        else if (30 < pt && pt < 40)
        {
            if (0.00  < aeta && aeta < 1.20) {return 0.971;}
            if (1.20  < aeta               ) {return 0.978;}
        }
        else if (40 < pt && pt < 50)
        {
            if (0.00  < aeta && aeta < 1.20) {return 0.978;}
            if (1.20  < aeta               ) {return 0.984;}
        }
        else if (pt > 50)
        {
            if (0.00  < aeta && aeta < 1.20) {return 0.974;}
            if (1.20  < aeta               ) {return 0.977;}
        }

        // if we get here, return bogus value
        cout << "[ss::TagAndProbeScaleFactor] WARNING -> missed coverage -- returning bogus value -999999" << endl;
        cout << Form("[ss::TagAndProbeScaleFactor] id: %d pt: %f eta: %f", id, pt, aeta) << endl; 
        return -999999.0;
    }

    // if we get here, return bogus value
    return -999999.0;
}

float DileptonTriggerScaleFactor(int hyp_type, const float trailing_pt, const float trailing_eta, bool is_reco){
    const bool is_barrel = (fabs(trailing_eta) < 1.0);
    const float lep_pt   = trailing_pt;

    //Square roots of dilepton values -- always high pT
    if (abs(hyp_type) == 11)   {return (lep_pt < 30 ? 0.959166 : 0.979795);} 
    if (abs(hyp_type) == 13) {return (is_barrel   ? 0.948683 : 0.90);} 
    return 0.0;
}  

