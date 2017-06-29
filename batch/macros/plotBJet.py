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
            ymin = 0.01

    for h in hists:
        if ymax:
            h.SetMaximum(scale*ymax)
            log.debug('normHists - ymax*scale=%f' %(scale*ymax))
        if ymin:            
            h.SetMinimum(ymin)

        if ymin and ymax and False:
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
    leg.SetFillStyle(0)
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
def getListOfRegions(rfile):

    dkeys = set()

    for tkey in rfile.GetListOfKeys():    
        rdir = rfile.Get(tkey.GetName())
    
        if not rdir:
            log.warning('getListOfRegions - missing dir: %s' %dsid)
            continue

        if not hasattr(rdir, 'GetListOfKeys'):
            log.warning('getListOfRegions - skip non TDirectory: %s' %dsid)
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

    text = "Unknown"

    if key.count('410000'):
        h.SetLineColor(2)
        text = "ttbar:"

    if key.count('361106'):
        h.SetLineColor(8)
        text = "Z #rightarrow ee:"

    if key.count('361107'):
        h.SetLineColor(4)
        text = "Z #rightarrow #mu#mu:"

    if key.count('361108'):
        h.SetLineColor(6)
        text = "Z #rightarrow #tau#tau:"
        
    if h.GetLineStyle() == 2:
        text += " fake b-jets"
        
    else:
        text += " real b-jets"

    return text

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
    h.GetYaxis().CenterTitle()

    return h

##############################################################################
def plot1d(rfiles, can, hpath):

    root_hists = []
    draw_hists = []

    for rfile in rfiles:

        h2path = hpath.replace('/bjet_real/', '/bjet_fake/')
        
        h  = getHist(rfile, hpath)
        h2 = getHist(rfile, h2path)

        if not h:
            log.warning('plot1d - failed to find h: %s:%s' %(rfile.GetName(), hpath))
            return
            
        if not h2:
            log.warning('plot1d - failed to find h2: %s:%s' %(rfile.GetName(), h2path))
            return

        h.SetStats(False)
        h.SetDirectory(0)
        h.SetLineWidth(2)
        
        h2.SetStats(False)
        h2.SetDirectory(0)
        h2.SetLineWidth(2)
        h2.SetLineStyle(2)

        h1min, h1max = getHistMinMax(h , True)
        h2min, h2max = getHistMinMax(h2, True)

        minimum = min(h1min, h2min)

        h .SetMinimum(minimum)
        h2.SetMinimum(minimum)

        draw_hists += [(h,  rfile.GetName())]
        draw_hists += [(h2, rfile.GetName())]
        root_hists += [h]
        root_hists += [h2]

    log.info('plot1d - %s and %s' %(hpath, h2path))

    # following hasn't been updated for overlaying:
    if options.do_unit_area and len(hists):
        for h in hists:
            if h.Integral() > 0.0:
                h1.Scale(1.0/h.Integral())
            else:
                log.warning('plot1d - zero integral for histogram')

    #------------------------------------------------------------------------

#    leg = getLegend(0.63, 0.70, 0.90, 0.88)
    leg = getLegend(0.59, 0.78, 0.96, 0.88)


    for i in range(0, len(draw_hists)):
        (h, key)  = draw_hists[i]
        
        if i == 0:
            h.Draw('HIST')
        else:
            h.Draw('SAME HIST')

        text = updateHistStyle(h, key)
        
        if text:
            leg.AddEntry(h, text, 'l')

    leg.Draw()

    normHists(root_hists)

    updateCanvas(can, hpath.replace('/', '_'))


##########################################################################
def main():         

    if options.save and options.outdir != None and not os.path.isdir(options.outdir):
        log.info('Make directory: %s' %options.outdir)
        os.makedirs(options.outdir)

    if len(args) == 0: 
        raise Exception('main - need at least one argument: %s' %(args))

    rfiles = []

    for arg in args:
        if not os.path.isfile(arg): 
            raise Exception('main - invalid input file: %s' %(arg))

        rfiles += [ROOT.TFile(arg, 'READ')]

    Config.loadPhysicsProjectLib(ROOT, options)

    regions = getListOfRegions(rfiles[0])

    can = makeCanvas('c')
    
    for r in regions:
      
        hists = getAllHistNames(rfiles[0], r)
            
        for h in hists:
            if h.startswith('bjet_real/'):
                plot1d(rfiles, can, '%s/plotBJet/%s' %(r, h))


##########################################################################
if __name__ == "__main__":
    main()
