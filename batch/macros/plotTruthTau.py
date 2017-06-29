#!/usr/bin/env python

import os
import re
import sys
import time
import optparse
from array import array

import PhysicsProject.PhysicsProjectConfig as Config

p = optparse.OptionParser()

p.add_option('--rebin',        type='int',    default=None,          dest='rebin')

p.add_option('-d', '--debug',   action='store_true', default=False,   dest='debug')
p.add_option('-w', '--wait',    action='store_true', default=False,   dest='wait')
p.add_option('-s', '--save',    action='store_true', default=False,   dest='save')
p.add_option('--logy',          action='store_true', default=False,   dest='logy')
p.add_option('--do-unit-area',  action='store_true', default=False,   dest='do_unit_area')
p.add_option('--do-plot',       action='store_true', default=False,   dest='do_plot')

p.add_option('--pref',         type='string', default=None,          dest='pref')
p.add_option('--post',         type='string', default=None,          dest='post')
p.add_option('--outdir', '-o', type='string', default=None,          dest='outdir')
p.add_option('--skey',         type='string', default='',            dest='skey')
p.add_option('--hkey',         type='string', default=None,          dest='hkey')
p.add_option('--rkey',         type='string', default=None,          dest='rkey')
p.add_option('--region',       type='string', default=None,          dest='region')

(options, args) = p.parse_args()

import ROOT

if not options.wait:
    ROOT.gROOT.SetBatch(True)

log = Config.getLog(os.path.basename(__file__), debug=options.debug)

canvases = {}

#=============================================================================
def updateCanvas(can, name=None):
    
    if not can:
        sys.exit(0)

    can.Modified()        
    can.Update()

    if options.wait:
        can.WaitPrimitive()

    if not can:
        sys.exit(0)
                
    if options.save and name != None:
        can.Print('%s.pdf' %getOutName(name), 'pdf')

#=============================================================================
def getOutName(name):

    if options.pref != None:
        name = '%s_%s' %(options.pref, name)

    if options.post != None:
        name = '%s_%s' %(name, options.post)

    if options.outdir != None:
        name = '%s/%s' %(options.outdir.rstrip('/'), name)
        
    return name

#=============================================================================
def getHistMinMax(hist, use_error=False):
    
    ymin = None    
    ymax = None

    if hist.Class().GetName().count('THStack'):
        return (hist.GetMinimum(), hist.GetMaximum())
    
    for ibin in range(1, hist.GetNbinsX()+1):
        val = hist.GetBinContent(ibin)
        err = hist.GetBinError  (ibin)

        if use_error:
            vmin = val - err
            vmax = val + err
        else:
            vmin = val
            vmax = val
        
        if vmax > 0.0:
            
            if ymax == None:
                ymax = vmax
            else:
                ymax = max([ymax, vmax])

        if vmin:
            
            if ymin == None:
                ymin = vmin
            else:
                ymin = min([ymin, vmin])

    return (ymin, ymax)

#=============================================================================
def normHists(hists, can=None, scale=1.27):
    
    ymax = None
    ymin = None    

    for h in hists:
        hmin, hmax = getHistMinMax(h, True)

        if ymax == None:
            ymax = hmax
        else:
            ymax = max([ymax, hmax])

        if ymin == None:
            ymin = h.GetMinimum()
        else:
            ymin = min([ymin, h.GetMinimum()])

    if can and can.GetLogy():
        scale = scale*3.0

        if not ymin > 0.0:
            ymin = 0.1

    if ymax != None: ymax *= scale
    if ymin != None: ymin /= scale

    log.info('ymax=%f' %ymax)
    log.info('ymin=%f' %ymin)

    for h in hists:
        if ymax != None:
            h.SetMaximum(scale*ymax)
            log.debug('normHists - ymax*scale=%f' %(scale*ymax))
        if ymin != None:            
            h.SetMinimum(ymin)

        if ymin != None and ymax != None:
            h.GetYaxis().SetRangeUser(ymin, ymax)

#=============================================================================
def getTopLatex(x, y, text, color=1):
    
    xlep = ROOT.TLatex(x, y, text)
    xlep.SetNDC()
    xlep.SetTextFont(42)
    xlep.SetTextSize(0.07)
    xlep.SetTextAlign(12)
    xlep.SetTextColor(color)
    xlep.Draw()

    return xlep

#=============================================================================
def getEffLatex(x, text, color=1):
    
    xlep = ROOT.TLatex(x, 0.93, text)
    xlep.SetNDC()
    xlep.SetTextFont(42)
    xlep.SetTextSize(0.045)
    xlep.SetTextAlign(12)
    xlep.SetTextColor(color)
    xlep.Draw()

    return xlep

#=============================================================================
def getLegend(x1, y1, x2, y2):
    
    leg = ROOT.TLegend(x1, y1, x2, y2)
    leg.SetBorderSize(0)
    leg.SetFillColor(0)
    leg.SetTextFont(42)
    leg.SetTextAlign(12) 

    return leg

#=============================================================================
def filterList(vals, key, val):

    if type(val) == type(''):
        return val.split(',')

    if key == None:
        return vals

    out = []

    for v in vals:
        if re.search(key, getattr(v, 'name', v)) != None:
            out += [v]

    return out

#=============================================================================
def getListOfRegions(rfile, dpath):

    dkeys = set()

    topdir = rfile.Get(dpath)
    
    if not topdir:
        log.warning('getListOfregions - missing region: %s' %dpath)
        return []

    for tkey in topdir.GetListOfKeys():    
        rdir = topdir.Get(tkey.GetName())
    
        if not rdir:
            log.warning('getListOfRegions - missing dir: %s' %tkey.GetName())
            continue

        if not hasattr(rdir, 'GetListOfKeys'):
            log.warning('getListOfRegions - skip non TDirectory: %s' %tkey.GetName())
            continue
        
        dkeys.add(tkey.GetName())
                                
    dkeys = filterList(sorted(list(dkeys)), options.rkey, options.region)

    log.info('getListOfRegions - found %d region(s)' %len(dkeys))

    for d in dkeys:
        log.info('   %s' %d)

    return dkeys

#=============================================================================
def getAllHistNames(rfile, hpath, hpref=''):

    hdir = rfile.Get(hpath)

    if not hdir or not hasattr(hdir, 'GetListOfKeys'):
        log.warning('getAllHistNames - failed to get TDirectory path: %s' %hpath)
        return []

    hnames = []

    for key in hdir.GetListOfKeys():
        n = key.GetName()

        if key.GetClassName().count('TH1') > 0:
            hname = '%s%s' %(hpref, n)
            
            if hname.count('CutCand'):
                continue

            if options.hkey and re.search(options.hkey, hname) == None:
                continue

            hnames += [hname]
        elif key.GetClassName().count('TDirectory'):
            hnames += getAllHistNames(rfile, '%s/%s' %(hpath.strip('/'), n), '%s/' %n)

    return hnames

#=============================================================================
def makeCanvas(name):
    
    global canvases
    
    if name in canvases:
        return canvases[name]

#    ROOT.gStyle.SetPadTopMargin(0.08)
    ROOT.gStyle.SetPadTopMargin(0.07)
#    ROOT.gStyle.SetPadRightMargin (0.10)         
    ROOT.gStyle.SetPadRightMargin (0.09)         
    ROOT.gStyle.SetPadLeftMargin (0.12)
#    ROOT.gStyle.SetPadBottomMargin(0.20)
    ROOT.gStyle.SetPadBottomMargin(0.15)



    can = ROOT.TCanvas(name, name, 650, 450)
    
    if options.logy:
        can.SetLogy(True)

    can.Draw()
    canvases[name] = can
    
    return can

##############################################################################
def updateHistStyle(h, key):
    
    if not h:
        log.debug('updateHistStyle - null pointer')
        return

    if key.count('410000'):
        h.SetLineColor(2)
        return "ttbar"

    if key.count('361106'):
        h.SetLineColor(8)
        return "Z #rightarrow ee"

    if key.count('361107'):
        h.SetLineColor(4)
        return "Z #rightarrow #mu#mu"

    if key.count('361108'):
        h.SetLineColor(6)
        return "Z #rightarrow #tau#tau"

    return "Unknown"

##############################################################################
def getHist(rfile, hpath):
    
    h = rfile.Get(hpath)
    
    if not h or h == None:
        log.warning('getHist - missing histogram: %s' %hpath)
        return None

    if h.Class().GetName().count('TH1') == 0 and h.Class().GetName().count('TH2') == 0:
        raise Exception('getHist - object is not histogram histogram: %s' %hpath)

    h.SetDirectory(0)
    h.SetLineWidth(2)
    h.SetStats(True)
    h.GetXaxis().CenterTitle()
    #h.GetYaxis().SetTitle("Number of occurrences")
    h.GetYaxis().CenterTitle()

    return h

##############################################################################
def plot1d(rfile, can, hpath_match, hpath_elec, hpath_mu, hpath_jet):

    hm = getHist(rfile, hpath_match)
    he = getHist(rfile, hpath_elec)
    hmu = getHist(rfile, hpath_mu)
    hj = getHist(rfile, hpath_jet)

    if not hm:
        log.warning('plot1d - failed to find hm: %s:%s' %(rfile.GetName(), hm))
        return

    if not he:
        log.warning('plot1d - failed to find he: %s:%s' %(rfile.GetName(), he))
        return


    if not hmu:
        log.warning('plot1d - failed to find hmu: %s:%s' %(rfile.GetName(), hmu))
        return

    if not hj:
        log.warning('plot1d - failed to find hj: %s:%s' %(rfile.GetName(), hj))
        return


    hm.SetStats(False)
    hm.SetLineColor(1) #black

    he.SetStats(False)
    he.SetLineColor(2) #red 

    hmu.SetStats(False)
    hmu.SetLineColor(3) #green
 
    hj.SetStats(False)
    hj.SetLineColor(4)  #blue

    #leg = getLegend(0.16, 0.78, 0.36, 0.90)
    leg = getLegend(0.71, 0.78, 0.91, 0.90)

    hm.Draw('HIST')
    he.Draw('HIST SAME')
    hmu.Draw('HIST SAME')
    hj.Draw('HIST SAME')


    leg.AddEntry(hm, 'Matched taus')
    leg.AddEntry(he, 'e')
    leg.AddEntry(hmu, 'mu')
    leg.AddEntry(hj, 'jet')


    leg.Draw()

    normHists([hm, he, hmu, hj], can)

    updateCanvas(can, hpath_match.replace('/', '_'))

##########################################################################
def main():         

    print args     

    if options.save and options.outdir != None and not os.path.isdir(options.outdir):
        log.info('Make directory: %s' %options.outdir)
        os.makedirs(options.outdir)

    if len(args) != 1: #argument is the file
        raise Exception('main - need exactly one argument: %s' %(args))

    rfile = ROOT.TFile(args[0], 'READ')

    Config.loadPhysicsProjectLib(ROOT, options)

    regions  = []
    regions += ['plot_prepCand_nocut']
    regions += ['plot_prepCand_1lep_1tau_2bjet'] 

    can = makeCanvas('c')
    
    for r in regions:
        print r 

        path_match = '%s/plotTruthTau/MatchedTauW' %r
        path_elec = '%s/plotTruthTau/ElecofNonMatchedTauW' %r
        path_mu = '%s/plotTruthTau/MuonofNonMatchedTauW' %r
	path_jet = '%s/plotTruthTau/JetofNonMatchedTauW' %r

        hists = getAllHistNames(rfile, path_match)
            
        for h in hists:
            hpath_match = '%s/%s' %(path_match, h)
            hpath_elec = '%s/%s' %(path_elec, h)
 	    hpath_mu = '%s/%s' %(path_mu, h)
	    hpath_jet = '%s/%s' %(path_jet, h)

            print '   Match histogram: %s' %hpath_match
            print '   Elec histogram: %s' %hpath_elec
	    print '   Muon histogram: %s' %hpath_mu
	    print '   Jet histogram: %s' %hpath_jet

            plot1d(rfile, can, hpath_match, hpath_elec, hpath_mu, hpath_jet)


##########################################################################
if __name__ == "__main__":
    main()
