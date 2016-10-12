import os
import math
import numpy as np

def smear(vals, errs, syst=0.0, only_normalization=False):
    """
    returns new_vals and new_errs after gaussian smearing by statistical 
    uncertainties added in quadrature with a relative (0.0,1.0) systematic
    uncertainty on the values.
    if only_normalization is True, then smearing will be done on the 
    overall normalization only
    """
    new_vals = []
    new_errs = []
    if syst < 0.001: only_normalization = False
    if only_normalization:
        tot = np.sum(vals)
        fact = max(np.random.normal(1.0, syst), 0.001)
        for val, err in zip(vals, errs):
            new_vals.append(val*fact)
            new_errs.append(err*fact)
    else:
        for val, err in zip(vals, errs):
            err = max(math.sqrt(err*2 + (syst*val)**2), 0.001)
            new_val = max(np.random.normal(val,err), 0.000)
            new_vals.append(new_val)
            new_errs.append(err*new_val/val)
    return np.array(new_vals), np.array(new_errs)

def calculate_chisq(vals_data, errs_data, vals_mc, errs_mc):
    vals_data, errs_data = np.array(vals_data), np.array(errs_data)
    vals_mc, errs_mc = np.array(vals_mc), np.array(errs_mc)
    vals_mc = np.sum(vals_mc, axis=0)
    errs_mc = np.sqrt(np.sum(errs_mc**2, axis=0))
    return np.sum((vals_data - vals_mc)**2/np.sqrt(errs_mc**2+errs_data**2))

def make_toys(vals_data,vals_wz,vals_ttz,vals_fakes,vals_rares,errs_data,errs_wz,errs_ttz,errs_fakes,errs_rares,ntoys=1000):
    min_chisq = 100.0
    sf_wz = 0.0
    sf_ttz = 0.0
    sf_fakes = 0.0
    sf_rares = 0.0
    # for i in tqdm(range(ntoys)):
    for _ in range(ntoys):
        new_vals_wz   , new_errs_wz    = smear(vals_wz    , errs_wz    , syst=0.5 , only_normalization=True)
        new_vals_ttz  , new_errs_ttz   = smear(vals_ttz   , errs_ttz   , syst=0.5 , only_normalization=True)
        new_vals_fakes, new_errs_fakes = smear(vals_fakes , errs_fakes , syst=0.0 , only_normalization=True)
        new_vals_rares, new_errs_rares = smear(vals_rares , errs_rares , syst=0.0 , only_normalization=False)

        chisq = calculate_chisq(vals_data, errs_data, 
                [new_vals_wz,new_vals_ttz,new_vals_fakes,new_vals_rares], 
                [new_errs_wz,new_errs_ttz,new_errs_fakes,new_errs_rares] )

        if chisq < min_chisq:
            min_chisq = chisq

            sf_wz = np.sum(new_vals_wz)/np.sum(vals_wz)
            sf_ttz = np.sum(new_vals_ttz)/np.sum(vals_ttz)
            sf_fakes = np.sum(new_vals_fakes)/np.sum(vals_fakes)
            sf_rares = np.sum(new_vals_rares)/np.sum(vals_rares)

    sfs = [sf_wz, sf_ttz, sf_fakes, sf_rares]
    vals = [new_vals_wz, new_vals_ttz, new_vals_fakes, new_vals_rares]
    return min_chisq, sfs, vals

def get_sfs(vals_data,vals_wz,vals_ttz,vals_fakes,vals_rares,errs_data,errs_wz,errs_ttz,errs_fakes,errs_rares,ntrials=5,ntoys=100):
    all_sfs = []
    for _ in range(ntrials):
        min_chisq, sfs, vals = make_toys(vals_data,vals_wz,vals_ttz,vals_fakes,vals_rares,errs_data,errs_wz,errs_ttz,errs_fakes,errs_rares,ntoys=ntoys) 
        all_sfs.append(sfs)


    central_sfs = np.mean(all_sfs, axis=0)
    errors_sfs = np.std(all_sfs, axis=0)
    all_sfs = np.array(all_sfs)
    return zip(central_sfs, errors_sfs)

if __name__ == "__main__":

    vals_data    =   [316.        ,  123.       ,  45.]
    errs_data    =   [17.77638883 ,  11.09053651,  6.70820393]
    vals_wz      =   [220.06199646,  24.15790749,  1.44199467]
    errs_wz      =   [3.73424567  ,  1.22148156 ,  0.28760846]
    vals_ttz     =   [17.75293541 ,  43.51371384,  27.45856667]
    errs_ttz     =   [0.58192646  ,  0.89499214 ,  0.68606607]
    vals_fakes   =   [14.8497715  ,  10.01793766,  0.88201594]
    errs_fakes   =   [1.63996567  ,  1.22733964 ,  0.43074784]
    vals_rares   =   [51.10387421 ,  22.60098839,  8.1201334]
    errs_rares   =   [3.62760224  ,  1.29745798 ,  0.38610714]

    print get_sfs(vals_data,vals_wz,vals_ttz,vals_fakes,vals_rares,errs_wz,errs_data,errs_ttz,errs_fakes,errs_rares)
