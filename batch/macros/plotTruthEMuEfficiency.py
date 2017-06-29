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

    #ROOT.gStyle.SetPadTopMargin(0.08)
    ROOT.gStyle.SetPadTopMargin(0.10)
    #ROOT.gStyle.SetPadRightMargin (0.10)         
    ROOT.gStyle.SetPadRightMargin (0.07) # for no 10e3
   # ROOT.gStyle.SetPadRightMargin (0.10) # for 10e3       
    #ROOT.gStyle.SetPadLeftMargin (0.12)
    ROOT.gStyle.SetPadLeftMargin (0.2) # when not using label offset
    #ROOT.gStyle.SetPadLeftMargin (0.19) # when using label offset
    #ROOT.gStyle.SetPadBottomMargin(0.20)
    ROOT.gStyle.SetPadBottomMargin(0.2)

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
        text = "t#bar{t}:"

    if key.count('361106'):
        h.SetLineColor(8)
        text = "Z #rightarrow ee"

    if key.count('361107'):
        h.SetLineColor(4)
        text = "Z #rightarrow #mu#mu"

    if key.count('361108'):
        h.SetLineColor(6)
        text = "Z #rightarrow #tau#tau"
        
    if h.GetLineStyle() == 2:
        text += " non-matched"
        
    else:
        text += " matched"

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
    
    #h.SetMinimum(1000)
    #h.SetMinimum(0)
    h.SetMinimum(0.5)
    #h.SetMaximum(10e2)
    
    h.GetYaxis().SetTitle("Number of occurrences")
    h.GetYaxis().CenterTitle()
    #h.GetYaxis().SetTitleOffset(1.5)

    return h

##############################################################################
def plot1d(rfiles, can, hpath):

    root_hists = []
    draw_hists = []

    for rfile in rfiles:
        
        if   '/ElecTruthMatchW1/' in hpath:
            h2path = hpath.replace('/ElecTruthMatchW1/', '/ElecTruthMatchW0/')

	    sDenom = 7313650
	    bDenom = 2120163

        elif '/MuonTruthMatchW1/' in hpath:
            h2path = hpath.replace('/MuonTruthMatchW1/', '/MuonTruthMatchW0/')

	    sDenom = 7959136
	    bDenom = 6021061

        elif '/TauTruthMatchW1/'  in hpath:
            h2path = hpath.replace('/TauTruthMatchW1/' , '/TauTruthMatchW0/' )
	    sDenom = 901265
	    bDenom = 226132

        else:
            h2path = hpath.replace('/BJetReal/'        , '/BJetFake/')
        
        h  = getHist(rfile, hpath)
        h2 = getHist(rfile, h2path)

        if not h:
            log.warning('plot1d - failed to find h: %s:%s' %(rfile.GetName(), hpath))
            return
            
        if not h2:
            log.warning('plot1d - failed to find h2: %s:%s' %(rfile.GetName(), h2path))
            return

	sInt = 0 # signal integral
 	bInt = 0 # background integral

	points = []
	Best = []

#Check if 0th bin is empty..
	for i in range(1, h.GetNbinsX()+1):
	    sInt += h .GetBinContent(i)
	    bInt += h2.GetBinContent(i)
	    points += [(sInt/sDenom, bInt/bDenom)]

#****PtVarCone40 for Muon
	    if sInt/sDenom >= 0.170:
	        Best += [(i)]

	if(len(Best)>0):
	    best_i = min(Best)
	    PtCone = (1.5/500)*(best_i -1 )
            print points[best_i -1][0]; print "PtCone: %f" % PtCone
	    print "Best_i: %d" % best_i
 	 
	h2d = ROOT.TH2D()
	h2d.SetBins(200, 0.158, sInt/sDenom, 200, 0.0026, bInt/bDenom)
	h2d.SetStats(False)
	h2d.GetXaxis().SetTitle("Signal efficiency")
	h2d.GetXaxis().CenterTitle()
	h2d.GetXaxis().SetTitleOffset(1.5)
	h2d.GetYaxis().SetTitle("Background efficiency")
	h2d.GetYaxis().CenterTitle()
	h2d.GetYaxis().SetTitleOffset(1.5)

	for point in points:
	    h2d.Fill(point[0], point[1])

        draw_hists += [(h2d,  rfile.GetName())]
     
    log.info('plot2d - %s and %s' %(hpath, h2path))

    # following hasn't been updated for overlaying:
    if options.do_unit_area and len(hists):
        for h in hists:
            if h.Integral() > 0.0:
                h1.Scale(1.0/h.Integral())
            else:
                log.warning('plot1d - zero integral for histogram')

    #------------------------------------------------------------------------

#    leg = getLegend(0.63, 0.70, 0.90, 0.88)
   # leg = getLegend(0.59, 0.78, 0.96, 0.88) #- this is top right
    leg = getLegend(0.58, 0.76, 0.95, 0.86) #- this is top right - one I've been using
    #leg = getLegend(0.28, 0.76, 0.65, 0.86) # top left
    
    #leg = getLegend(0.49, 0.28, 0.86, 0.38) # bottom right


    for i in range(0, len(draw_hists)):
        (h, key)  = draw_hists[i]
        
        if i == 0:
            h.Draw('COL')
        else:
            h.Draw('SAME HIST')

#        text = updateHistStyle(h, key)
        
#        if text:
#            leg.AddEntry(h, text, 'l')

#    leg.Draw()

    #normHists(root_hists)
    
#    updateCanvas(can, hpath.replace('/', '_'))


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
            if h.startswith('ElecTruthMatchW1/') or h.startswith('MuonTruthMatchW1/') or h.startswith('TauTruthMatchW1/') or h.startswith('BJetReal/'):
                plot1d(rfiles, can, '%s/plotRecoLep/%s' %(r, h))


##########################################################################
if __name__ == "__main__":
    main()
