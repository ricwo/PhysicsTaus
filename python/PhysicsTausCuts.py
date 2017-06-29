
import os
import sys
import re
import PhysicsBase .PhysicsBaseConfig  as physicsBase
import PhysicsTaus.PhysicsTausConfig as physicsLight

from PhysicsTaus.PhysicsTausConfig import CutItem

clog = physicsLight.getLog(__name__)

#-----------------------------------------------------------------------------
def addCuts(alg, key, cuts, save=False):

    if alg == None:
        raise Exception('addCuts - invalid alg')

    if type(cuts) != type([]):
        raise Exception('addCuts - invalid inputs for %s: %s, %s, %s' %(alg.GetAlgName(), key, cuts, dir(cuts)))

    keys = []
    
    for cut in cuts:
        print cut
        if cut.GetCutName() in keys:
            raise Exception('addCuts - %s::%s - duplicate cut %s in %s' %(alg.GetAlgName(), key, cut.GetCutName(), keys))

        keys += [cut.GetCutName()]
        alg.SetPar('%s%s'  %(key, cut.GetCutName()), cut.GetRegistry())        

    alg.SetPar(key, ','.join(keys))
    
    if save:
        clog.info('Print cuts: %s' %key)
        print physicsLight.getCutsAsLatex(cuts, 'cuts_%s_%s.tex' %(alg.GetAlgName(), key))

    return cuts

#-----------------------------------------------------------------------------
def addCutsToRegistry(reg, key, cuts):

    if reg == None:
        raise Exception('addCutsToRegistry - invalid reg')

    if type(cuts) != type([]):
        raise Exception('addCutsToRegistry - invalid inputs: %s, %s, %s' %(key, cuts, dir(cuts)))

    keys = []
    
    for cut in cuts:
        if cut.GetCutName() in keys:
            raise Exception('addCutsToRegistry - %s - duplicate cut %s in %s' %(key, cut.GetCutName(), keys))

        keys += [cut.GetCutName()]
        physicsBase.setRegistryKey(reg, '%s%s'  %(key, cut.GetCutName()), cut.GetRegistry())

    physicsBase.setRegistryKey(reg, key, ','.join(keys))

    return cuts

#-----------------------------------------------------------------------------
def getRunChain(name, algs=None, cuts=None, options=None):
    
    alg = physicsBase.AlgConfig(name, 'RunChain')

    if type(cuts) == type([]):
        addCuts(alg, 'CutCand', cuts)
    
    if type(algs) == type([]) and len(algs):
        alg.AddAlg(algs)

    if options != None and cuts != None and options.print_cuts:
        clog.info('----------------------------------------')
        clog.info('Print candidate cuts for %s:' %name)
        print physicsLight.getCutsAsLatex(cuts, 'RunChain_%s_CutCand.tex' %name)

    return alg
