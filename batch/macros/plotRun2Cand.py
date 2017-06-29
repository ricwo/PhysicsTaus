#!/usr/bin/env python

from optparse import OptionParser

p = OptionParser(usage='usage: <path:ROOT file directory>')

p.add_option('-o', '--outdir',     type='string', default='plots',       dest='outdir')
p.add_option('--hkey',             type='string', default=None,          dest='hkey')
p.add_option('--rkey',             type='string', default=None,          dest='rkey')

p.add_option('--pref',             type='string', default=None,          dest='pref')
p.add_option('--post',             type='string', default=None,          dest='post')
p.add_option('--outfile',          type='string', default=None,          dest='outfile')
p.add_option('--region',           type='string', default=None,          dest='region')
p.add_option('--regions',          type='string', default=None,          dest='regions')
p.add_option('--sub-regions',      type='string', default=None,          dest='sub_regions')
p.add_option('--dkey',             type='string', default=None,          dest='dkey')
p.add_option('--xsec-list',        type='string', default=None,          dest='xsec_list')
p.add_option('--obj-file',         type='string', default=None,          dest='obj_file')
p.add_option('--data-file',        type='string', default=None,          dest='data_file')
p.add_option('--counts-dir',       type='string', default='Counts',      dest='counts_dir')
p.add_option('--config-path',      type='string', default=None,          dest='config_path')

p.add_option('--file-dsid',    type='string', default='yields_per_mcchan.txt', dest='file_dsid')
p.add_option('--file-sample',  type='string', default='yields_per_sample.txt', dest='file_sample')

p.add_option('--ilumi',        type='float',  default=3209.05,       dest='ilumi')
p.add_option('--rebin',        type='int',    default=None,          dest='rebin')
p.add_option('--nregion',      type='int',    default=None,          dest='nregion')
p.add_option('--scale-tth',    type='int',    default=None,          dest='scale_tth')
p.add_option('--scale-fakes',  type='float',  default=None,          dest='scale_fakes')

p.add_option('-b', '--batch',   action='store_true', default=False,   dest='batch')
p.add_option('-d', '--debug',   action='store_true', default=False,   dest='debug')
p.add_option('-w', '--wait',    action='store_true', default=False,   dest='wait')
p.add_option('-s', '--save',    action='store_true', default=False,   dest='save')
p.add_option('--save-tex',      action='store_true', default=False,   dest='save_tex')
p.add_option('--stack',         action='store_true', default=False,   dest='stack')
p.add_option('--wait-last',     action='store_true', default=False,   dest='wait_last')
p.add_option('--zoom',          action='store_true', default=False,   dest='zoom')
p.add_option('--logy',          action='store_true', default=False,   dest='logy')

p.add_option('--do-sig',        action='store_true', default=False,   dest='do_sig')
p.add_option('--do-ratio', '-r',action='store_true', default=False,   dest='do_ratio')
p.add_option('--do-jet',        action='store_true', default=False,   dest='do_jet')
p.add_option('--do-lep',        action='store_true', default=False,   dest='do_lep')
p.add_option('--do-tau',        action='store_true', default=False,   dest='do_tau')
p.add_option('--do-png',        action='store_true', default=False,   dest='do_png')
p.add_option('--do-eps',        action='store_true', default=False,   dest='do_eps')
p.add_option('--show-mean',     action='store_true', default=False,   dest='show_mean')
p.add_option('--show-input',    action='store_true', default=False,   dest='show_input')
p.add_option('--no-plots',      action='store_true', default=False,   dest='no_plots')

p.add_option('--add-runs',        action='store_true', default=False,   dest='add_runs')
p.add_option('--no-title',        action='store_true', default=False,   dest='no_title')
p.add_option('--no-fill',         action='store_true', default=False,   dest='no_fill')
p.add_option('--no-norm',         action='store_true', default=False,   dest='no_norm')
p.add_option('--no-data',         action='store_true', default=False,   dest='no_data')
p.add_option('--calc-signi',      action='store_true', default=False,   dest='calc_signi')
p.add_option('--obj-cutflow',     action='store_true', default=False,   dest='obj_cutflow')
p.add_option('--no-xsec-weight',  action='store_true', default=False,   dest='no_xsec_weight')
p.add_option('--get-regions',     action='store_true', default=False,   dest='get_regions')
p.add_option('--get-subdirs',     action='store_true', default=False,   dest='get_subdirs')
p.add_option('--note-style',      action='store_true', default=False,   dest='note_style')
p.add_option('--do-sample-table', action='store_true', default=False,   dest='do_sample_table')


(options, args) = p.parse_args()  

import copy
import math
import os
import re
import sys
import logging
import string
import ROOT

import PhysicsProject.PhysicsProjectConfig as Config
import PhysicsProject.PhysicsProjectXSec   as XSec
import PhysicsProject.PhysicsProjectInfo   as ttH

if options.batch or not options.wait:
    ROOT.gROOT.SetBatch(True)

log = Config.getLog(__name__, debug=options.debug)

#================================================================================

class OneCol:    
    def __init__(self, name, title, hist, no_err=False, no_dec=False, do_sqrt=False):

        self.name   = name
        self.title  = title
        self.hist   = hist
        self.cells  = {}
        self.cellw  = len(title)
        self.labelw = 0        
        self.no_err = no_err
        self.do_sqrt = do_sqrt

        for i in range(1, hist.GetNbinsX()+1):
            lab  = hist.GetXaxis().GetBinLabel(i)

            if self.no_err:
                if self.do_sqrt:
                    val  = '%.0f' %math.sqrt(hist.GetBinContent(i))
                    cell = '$%s$' %(val)
                else:
                    val  = '%.0f' %hist.GetBinContent(i)
                    cell = '$%s$' %(val)
            else:
                if self.do_sqrt:
                    if hist.GetBinContent(i) < 0:
                        hist.SetBinContent(i,0.0)
                    if no_dec:
                        val  = '%.0f' %math.sqrt(hist.GetBinContent(i))
                        err  = '%.0f' %math.sqrt(hist.GetBinError  (i))
                    else:
                        val  = '%.2f' %math.sqrt(hist.GetBinContent(i))
                        err  = '%.2f' %math.sqrt(hist.GetBinError  (i))                    
                else:                    
                    if no_dec:
                        val  = '%.0f' %hist.GetBinContent(i)
                        err  = '%.0f' %hist.GetBinError  (i)
                    else:
                        val  = '%.2f' %hist.GetBinContent(i)
                        err  = '%.2f' %hist.GetBinError  (i)                    
                
                cell = '$%s \\pm %s$' %(val, err)
            
            self.cells[i] = cell
            
            self.cellw  = max([self.cellw,  len(cell)])
            self.labelw = max([self.labelw, len(lab)])
            
            #
            # Normalize cells
            #
            for i, cell in self.cells.iteritems():
                d = self.cellw - len(cell)
                
                if d > 0:
                    self.cells[i] = string.rjust(cell, self.cellw, ' ')

    def GetNCell(self):
        return len(self.cells)

    def GetLastValueError(self):
        val = self.hist.GetBinContent(self.hist.GetNbinsX())
        err = self.hist.GetBinError(self.hist.GetNbinsX())
        
        return (val, err)

    def GetCell(self, icell):
        try:
            return self.cells[icell]
        except KeyError:
            print '----------------------------------------------------------------'
            print self.cells
            self.hist.Print()

            raise Exception('GetCell %s - index out of range: %s' %(self.title, icell))            

    def GetColTitle(self):
        return string.ljust(self.title, self.cellw, ' ')

    def GetCutName(self, icell):
        if icell == None:
            lab = ''
        else:

            lab = '%s' %self.hist.GetXaxis().GetBinLabel(icell)
            
        return string.ljust(lab, self.labelw, ' ')

    def GetHist(self):
        return self.hist

    def GetName(self):
        return self.name

    def GetTitle(self):
        return self.title

    def PrintCells(self):
        for i, cell in self.cells.iteritems():
            print '\'%s\'' %cell

#================================================================================
def getCutFlowHist(sman, rfile, region, hist, sname, is_data=False):
    
    if is_data:
        h = getData(dfile, '%s/%s' %(region, hist), True)
    else:
        h = sman.GetCutFlow(rfile, '%s/%s' %(region, hist), sname)

    do_data_norm = region.count('plotCand_2l_1tau_all_TauMedium_SS_4jet_1bjet') and sname == 'tops'
    if not do_data_norm:
        return h

    dnorm = getTopNorm(region)
    derr  = getTopNormErrorRelative(region)
    
    nbin = h.GetNbinsX()
    h.SetBinContent(nbin,  dnorm)
    h.SetBinError(  nbin,  dnorm*derr)
    
    return h

#================================================================================
def writeTextTable(table, region, fpath):

    ''' Write text file with event yields per region and sample'''

    nwid = 0

    for col in table:
        nwid = max([nwid, len(col.GetName())])

    for col in table:
        (val, err) = col.GetLastValueError()
        line = '%s  %s  %-20s %-20s' %(col.GetName().ljust(nwid), region, '%f' %val, '%f' %err)
        appendLine(getOutName(options.file_sample), line)
    
#================================================================================
def prepLatexTable(table, first):

    #
    # Make header row
    #
    lkeys  = []
    header = first.GetCutName(None)
    
    for col in table:
        header += ' & %s' % (col.GetColTitle())

    header += '\\\\\n'

    #
    # Make table rows
    #
    rows = []
    
    for i in range(1, first.GetNCell()+1):
        if first.GetCutName(i).strip() in lkeys:
            rows += ['\\hline']
            
        row = first.GetCutName(i)

        for col in table:
            row += ' & %s' % (col.GetCell(i))

        rows += [row + '\\\\']

    #
    # Print latex table
    #
    text  = '\\resizebox{1.0\\textwidth}{!}{\n'
    text += '\\begin{tabular}{|c|'

    for col in table:
        text += 'c|'

    text += '}\n'
    text += '\\hline\n'
    text += header    
    text += '\\hline\n'
    
    for row in rows:
        text += row + '\n'

    text += '\\hline\n'
    text += '\\end{tabular}}\n'
    
    return text

#================================================================================
def makeTable(rfile, dfile, sman, sig_samples, bkg_samples, oth_samples, region, hist):

    log.info('makeTable -  %s/%s number of samples: %d' %(region, hist, len(sig_samples+bkg_samples+oth_samples)))

    for sample in sig_samples+bkg_samples:
        log.debug('   %-20s' %(sample.name))

    table     = []
    table_oth = []
    first_sig = None
    first_bkg = None
    total_sig = None
    total_bkg = None
    
    #----------------------------------------------------------------------------------------
    # Signal samples - h is cutflow histogram of region and sample - usually named 'CutCand'
    #
    for sample in sig_samples:
        h = getCutFlowHist(sman, rfile, region, hist, sample.name)

        if not h:
            log.warning('makeTable - missing histogram %s: %s' %(hist, sample.name))
            continue

        col = OneCol(sample.name, sample.title, h)
        table += [col]

        if first_sig == None:
            first_sig = col
            total_sig = h.Clone()
            total_sig.SetDirectory(0)
        else:
            total_sig.Add(h)
    
    #----------------------------------------------------------------------------------------
    # bkg samples - h is cutflow histogram of region and sample - usually named 'CutCand'
    #
    for sample in bkg_samples:
        h = getCutFlowHist(sman, rfile, region, hist, sample.name)

        if not h:
            log.warning('makeTable - missing histogram %s: %s' %(hist, sample.name))
            continue

        col = OneCol(sample.name, sample.title, h)
        table += [col]

        if first_bkg == None:
            first_bkg = col
            total_bkg = h.Clone()
            total_bkg.SetDirectory(0)
        else:
            total_bkg.Add(h)

    #----------------------------------------------------------------------------------------
    # oth samples - h is cutflow histogram of region and sample - usually named 'CutCand'
    #
    for sample in oth_samples:
        h = getCutFlowHist(sman, rfile, region, hist, sample.name)

        if not h:
            log.warning('makeTable - missing histogram %s: %s' %(hist, sample.name))
            continue

        col = OneCol(sample.name, sample.title, h)
        table_oth += [col]

    #    if first_sig and first_bkg == None:
    #        log.warning('makeTable - failed to read cuts')
    #        return None

    #
    # Add total background and data and S/sqrt(B)
    #    
    if total_bkg:
        table += [OneCol('sum_bkg', 'Sum bkg', total_bkg)]

    if options.calc_signi and total_sig and total_bkg:
        h_signi = total_sig.Clone()
        h_signi.Multiply(total_sig)
        h_signi.Divide(total_bkg)
        table += [OneCol('s_over_sqrt_b','S/sqrt(B)', h_signi, do_sqrt=True)]

    #
    # Get data cut-flow
    #
    hdata = getCutFlowHist(sman, dfile, region, hist, 'data', True)
    
    if hdata and not options.no_data:
        table += [OneCol('data', 'Data', hdata, no_err=True)]

        if total_bkg:
            total_rat = hdata.Clone('%s_ratio' %hdata.GetName())
            total_rat.SetDirectory(0)            
            total_rat.Divide(total_bkg)
        
            table += [OneCol('data_over_mc', 'Data/MC', total_rat)]

    #
    # Make tables
    #
    if first_sig: 
        text = prepLatexTable(table, first_sig)
    elif first_bkg:
        text = prepLatexTable(table, first_bkg)
    else:
        return None

    writeTextTable(table+table_oth, region, '')

    if options.save or options.save_tex:
        if hist == 'CutCand':
            ofile = open(getOutName('%s.tex' %region), 'w')
            ofile.write(text)
            ofile.close()
        else:
            ofile = open(getOutName('%s_%s.tex' %(region,hist)), 'w')
            ofile.write(text)
            ofile.close()

    print text    
    return text

#================================================================================
def appendLine(spath, text):

    if spath:
        sfile = open(spath, 'a')
        sfile.write('%s\n' %text)
        sfile.close()

#================================================================================
def makeSampleTable(rfile, dfile, sman, samples, region, hist):

    log.info('makeSampleTable -  %s/%s number of samples: %d' %(region, hist, len(samples)))

    rhists = {}
    dsids  = []

    for sample in samples:
        log.debug('   %-20s' %(sample.name))

        hruns = sman.GetSampleRunHists(rfile, '%s/NJet' %region, sample.name)
        rhists.update(hruns)

    for run in sorted(rhists.keys()):
        hist = rhists[run]
        rstr = '%s %s %6d  %f' %(run, region, hist.GetEntries(), hist.Integral())
        dsids += ['%s' %run]

        appendLine(getOutName(options.file_dsid), rstr)
        
    return sorted(dsids)

#================================================================================
def prepObjTable(col, rfile, sman, samples, hist):

    #
    # Make header row
    #
    header = col.GetCutName(None)
    header += ' & %s' % (col.GetColTitle())
    header += '\\\\\n'

    #
    # Make table rows
    #
    rows = []
    for i in range(1, col.GetNCell()+1):
        row = col.GetCutName(i)
        row += ' & %s' % (col.GetCell(i))
        rows += [row + '\\\\']

    #
    # Print latex table
    #

    text  = '\\resizebox{1.0\\textwidth}{!}{\n'
    text += '\\begin{tabular}{|c|'
    
    text += '}\n'
    text += '\\hline\n'
    text += header    
    text += '\\hline\n'
    
    for row in rows:
        text += row + '\n'

    text += '\\hline\n'
    text += '\\end{tabular}}\n'
   
    return text

#================================================================================
def makeObjTable(rfile, sman, samples, region, hist):

    log.info('makeTable -  %s/%s' %(region, hist))
    
    h = GetRawHisto(rfile, region, hist)
    
    if not h:
        log.warning('makeTable - missing histogram %s: %s' %(region,hist))
    else:
        col = OneCol(h.GetName(), h.GetName(), h)
            
    #
    # Make header row
    #
    if h: 
        text = prepObjTable(col, rfile, sman, samples, hist)
    else:
        return None
        
    if options.save or options.save_tex:
        ofile = open(getOutName('%s_%s.tex' %(region,hist)), 'w')
        ofile.write(text)
        ofile.close()

    print text    
    return text

#================================================================================
def GetRawHisto(rfile, region, hist):

    histo = None
    path = '%s/%s' %(region, hist)
    histo = rfile.Get(path)

    return histo

#================================================================================
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

#================================================================================
def getOutName(name):

    if name == None:
        return None

    if options.pref != None:
        name = '%s_%s' %(options.pref, name)

    if options.post != None:
        name = '%s_%s' %(name, options.post)

    if options.outdir != None:
        name = '%s/%s' %(options.outdir.rstrip('/'), name)
        
    return name

#================================================================================
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

#================================================================================
def getYaxisMinMax(hists, use_error):
    
    ymin = None
    ymax = None    

    for h in hists:
        hmin, hmax = getHistMinMax(h, use_error)
        
        if ymax == None:
            ymax = hmax
        else:
            ymax = max([hmax, ymax])

        if ymin == None:
            ymin = hmin
        else:
            ymin = min([hmin, ymin])
    
    return (ymin, ymax)

#================================================================================
def getRangeIntegral(hist, xmin, xmax):

    sum = 0.0

    for ibin in range(0, hist.GetNbinsX()+1):
        pos = hist.GetXaxis().GetBinCenter(ibin)
        val = hist.GetBinContent(ibin)
        
        if pos > xmin and pos < xmax:
            sum += val
    
    return sum

#-------------------------------------------------------------------------
def updatePad(pad, use_error=True):
    
    plist = pad.GetListOfPrimitives()
    hists = []
    
    stath = 0.25
    staty = 1.0
    statw = 0.20
    statx = 0.80
    
    for p in plist:
        if not p.InheritsFrom('TH1'):
            continue
        
        hists.append(p)
        stats = p.FindObject('stats')
        
        if stats:
            stats.SetTextColor(p.GetLineColor())
            stats.SetY1NDC(staty-stath)
            stats.SetY2NDC(staty)
            stats.SetX2NDC(statx+statw)
            stats.SetX1NDC(statx)            
            staty = staty - stath

    #
    # Update Y-axis range
    #
    (ymin, ymax) = getYaxisMinMax(hists, use_error)

    if ymax != None:
        for h in hists:
            h.SetMaximum(1.1*ymax)

    if ymin != None and ymin > 0.0:
        for h in hists:
            h.SetMinimum(0.9*ymin)
    
#================================================================================
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
        name = getOutName(name)

        if   options.do_png:
            can.Print('%s.png' %name, 'png')
        elif options.do_eps:
            can.Print('%s.eps' %name, 'eps')
        else:
            can.Print('%s.pdf' %name, 'pdf')        

#================================================================================
def makeCanvas(name):
    
    if not options.do_ratio:

        ROOT.gStyle.SetPadRightMargin (0.09)         
        ROOT.gStyle.SetPadBottomMargin(0.18)
    
        can = ROOT.TCanvas(name, name, 650, 450)

        if options.logy:
            can.SetLogy(True)
        
        return can

    tmarg = ROOT.gStyle.GetPadTopMargin()    
    rmarg = ROOT.gStyle.GetPadRightMargin()
    bmarg = ROOT.gStyle.GetPadBottomMargin()
    
    can = ROOT.TCanvas(name, name, 850, 650)
    can.Divide(2, 1, 0.02, 0.01, 0)
    
    ROOT.gStyle.SetPadRightMargin(0.07)    

    if options.note_style:
        ROOT.gStyle.SetPadBottomMargin(0.00)
        ROOT.gStyle.SetPadLeftMargin  (0.13)    
    else:
        ROOT.gStyle.SetPadBottomMargin(0.04)
        ROOT.gStyle.SetPadLeftMargin  (0.12)
    
    pad_top = can.cd(1)
    pad_top.SetPad(0.00, 0.40, 1.0, 1.00)
    pad_top.SetFillStyle(0)
    pad_top.SetFillColor(10)
    pad_top.UseCurrentStyle()
    
    if options.logy:
        pad_top.SetLogy(True)

    if options.note_style:
        ROOT.gStyle.SetPadTopMargin(0.02)
    else:
        ROOT.gStyle.SetPadTopMargin(0.04)

    ROOT.gStyle.SetPadBottomMargin(0.26)
    
    pad_low = can.cd(2)
    pad_low.SetPad(0.00, 0.00, 1.0, 0.40)
    pad_low.SetFillStyle(0)
    pad_low.SetFillColor(10)
    pad_low.UseCurrentStyle()
    
    ROOT.gStyle.SetPadTopMargin(tmarg)
    ROOT.gStyle.SetPadRightMargin(rmarg)
    ROOT.gStyle.SetPadBottomMargin(bmarg)    
    
    return can

#================================================================================
def normHists(hists, can=None, scale=1.05):
    
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
        if ymin:            
            h.SetMinimum(ymin)
            
#================================================================================
def getTopLatex(text):
    
    xlep = ROOT.TLatex(0.55, 0.96, text)
    xlep.SetNDC()
    xlep.SetTextFont(42)
    xlep.SetTextSize(0.05)
    xlep.SetTextAlign(12)
    xlep.Draw()

    return xlep

#================================================================================
def getRunHists(sman, rfile, samples, dpath):

    ohists = []

    for s in samples:
    
        rhists = sman.GetSampleRunHists(rfile, '%s/NJet' %dpath, s.name)
        
        nbin  = max([1, len(rhists)])
        rname = 'runs_%s_%s' %(dpath.replace('/', '_'), s.name)

        log.info('getRunHists - %s: %s - #hists=%d' %(dpath, s.name, len(rhists)))

        rhist = ROOT.TH1D(rname, rname, nbin, 0.0, nbin)
        rhist.SetDirectory(0)

        ohists += [rhist]

        ibin = 1

        for run, hist in rhists.iteritems():
            rhist.SetBinContent(ibin, hist.Integral())
            rhist.GetXaxis().SetBinLabel(ibin, '%s' %run)

            ibin += 1
            
    return ohists

#================================================================================
def getTopNorm(region):

    if region.count('plotCand_2l_1tau_all_TauMedium_SS_4jet_1bjet'):
        return 0.61

def getTopNormErrorRelative(region):
    central = 0.61
    up   = math.sqrt( pow(0.55,2)  + pow(0.33,2) )
    down = math.sqrt( pow(-0.31,2) + pow(0.33,2) )
    symmetrised_e = (up+down)/2

    return symmetrised_e/central
    
#================================================================================
def getHists(sman, rfile, samples, dpath, hist):

    dnorm = getTopNorm(dpath)
    hpath = '%s/%s' %(dpath, hist)
    hists = []
    
    for s in samples:
        hist = sman.GetHist(rfile, hpath, s.name)

        if not hist:
            if options.debug:
                log.debug('getHists - miss histogram for %s: %s' %(s.name, hpath))
            continue

        if dnorm  and dnorm > 0.0:
            if s.name.count('tops'):
                log.info('getHists - %s: scale to data prediction nevent=%f' %(s.name, dnorm))                
                hist.Scale(dnorm/hist.Integral())
                for i in range(0,hist.GetNbinsX()+1):
                    bc = hist.GetBinContent(i)
                    e = getTopNormErrorRelative(dpath)
                    hist.SetBinError(i,bc*e)
        
        if options.rebin:
            hist.Rebin(options.rebin)

        if options.debug:
            log.info('getHists - %s: %-12s  N_{bkg}=%10s' %(hpath, s.name, ('%0.1f' % hist.Integral()).rjust(10)))
        
        hists.append((hist, s))

    return hists

#================================================================================
def getData(dfile, hpath, is_cutflow=False):

    if options.no_data:
        return None

    path = 'data/%s' %(hpath)
    hist = dfile.Get(path)
            
    if not hist:
        log.warning('getData - missing data histogram: %s' %path)
        return None

    hist = hist.Clone()
    hist.SetDirectory(0)

    hist.SetLineWidth  (0)
    hist.SetMarkerStyle(20)
    hist.SetMarkerColor(1)        

    if not is_cutflow and options.rebin:
        hist.Rebin(options.rebin)

    return hist

#================================================================================
def plotHist(rfile, dfile, can, sman, sig_samples, bkg_samples, oth_samples, dpath, hist):

    do_fakes_in_legend = False
    if dpath.count('plotCand_2l_1tau_all_TauMedium_SS_4jet_1bjet'):
        do_fakes_in_legend = True
        
    if options.do_ratio:
        pad = can.cd(1)
    else:
        can.cd()
        pad = can

    hpath     = '%s/%s' %(dpath, hist)
    hname     = hpath.replace('/', '_')

    log.info('plotHist  - %s' %hpath)

    out_hists = []
    bkg_hists = getHists(sman, rfile, bkg_samples, dpath, hist)
    sig_hists = getHists(sman, rfile, sig_samples, dpath, hist)
    oth_hists = getHists(sman, rfile, oth_samples, dpath, hist)
    hdata     = getData (dfile, hpath)

    if hdata:
        hdata.SetName('%s_data' %hname)

    log.debug('plotHist - histogram path: %s' %(hpath))

    if options.note_style:
        if options.do_ratio:
            leg = ROOT.TLegend(0.78, 0.12, 0.95, 0.65)
        else:
            leg = ROOT.TLegend(0.78, 0.38, 0.90, 0.71)
    else:
        if options.do_ratio:
            leg = ROOT.TLegend(0.75, 0.68, 0.93, 0.90)
        else:
            leg = ROOT.TLegend(0.72, 0.72, 0.92, 0.89)
  
    leg.SetBorderSize(0)
    leg.SetFillStyle (0)
    leg.SetNColumns(2)

    stack = ROOT.THStack('hstack', 'hstack')
    hbkg  = None
    hsig  = None    

    for entry in bkg_hists:
        hist   = entry[0]
        sample = entry[1]
        
        hist.SetName('%s_%s' %(hname, sample.name))
        hist.SetDirectory(0)

        out_hists += [hist]

        if not options.note_style:
            leg.AddEntry(hist, sample.title, 'f')
        
        stack.Add(hist)

        if not hbkg:
            hbkg = hist.Clone('%s_bkg' %hname)
            hbkg.SetDirectory(0)
        else:
            hbkg.Add(hist)
    
    stack.Draw('HIST')
    all_hists = [stack]

    if hbkg:
        stack.GetHistogram().GetXaxis().SetTitle(hbkg.GetXaxis().GetTitle())

        if options.note_style:
            if options.do_ratio:
                stack.GetXaxis().SetLabelSize(0.0)

            stack.GetHistogram().GetYaxis().SetTitle('Entries')        
            stack.GetHistogram().SetTitleOffset(1.05,'Y')
        else:            
            if len(hbkg.GetYaxis().GetTitle()):        
                stack.GetHistogram().GetYaxis().SetTitle(hbkg.GetYaxis().GetTitle())
            else:
                stack.GetHistogram().GetYaxis().SetTitle('Events')                

            stack.GetHistogram().GetYaxis().SetTitleOffset(0.8)
            stack.GetHistogram().GetXaxis().CenterTitle()
            stack.GetHistogram().GetYaxis().CenterTitle()

    #
    # Add shaded error histogram
    #
    if hbkg:
        hstats = hbkg.Clone('%s_stats' %hbkg.GetName())

    if options.note_style:
        if hbkg:    
            hstats.SetMarkerStyle(0);
            hstats.SetFillColor(1);
            hstats.SetLineColor(1);
            hstats.SetFillStyle(3005);
            hstats.Draw('SAME E2')

    for entry in sig_hists:
        all_hists += [entry[0]]

    for entry in sig_hists:
        hist   = entry[0]
        sample = entry[1]
        
        hist.SetName('%s_%s' %(hname, sample.name))
        hist.SetDirectory(0)

        if options.scale_tth:
            hist.Scale(options.scale_tth)
       
        all_hists += [hist]
        
        hist.Draw('SAME HIST')

        if not options.note_style:
            if options.scale_tth:
                leg.AddEntry(hist, '%s (x%s)' %(sample.title,options.scale_tth),'l')
            else: 
                leg.AddEntry(hist, sample.title, 'l')

        if not hsig:
            hsig = hist.Clone('%s_sig' %hname)
            hsig.SetDirectory(0)
        else:
            hsig.Add(hist)        

    for entry in oth_hists:
        hist   = entry[0]
        sample = entry[1]
        
        hist.SetName('%s_%s' %(hname, sample.name))
        hist.SetDirectory(0)

        out_hists += [hist]

    if hdata:
        all_hists += [hdata]
        hdata.Draw('pSAME')
        leg.AddEntry(hdata, 'Data', 'p')

    if options.note_style:
        for entry in sig_hists:
            if options.scale_tth:
                leg.AddEntry(entry[0], '%s (x%s)' %(entry[1].title,options.scale_tth),'l')
            else: 
                leg.AddEntry(entry[0], entry[1].title, 'l')
        for entry in reversed(bkg_hists):
            if entry[1].name.count('tops') and do_fakes_in_legend:
                leg.AddEntry(entry[0], "Fakes", "f")
            else:
                leg.AddEntry(entry[0], entry[1].title, 'f')
        if hstats:
            leg.AddEntry(hstats, 'stat uncert', 'f')

    if options.note_style:
        normHists(all_hists, can, 1.30)
    else:
        normHists(all_hists, can, 1.35)

    leg.Draw()

    #
    # Draw Text - ToDo: adjust channel_name accordingly
    #
    if options.note_style:
        channel_name = '2l + #tau_{had} channel'
        Text = GetText(channel_name)
        for txt in Text:
            txt.Draw()

    #
    # Draw histogram mean
    #
    if options.show_mean and hsig and hbkg:
        text = '<sig>=%.2f  <bkg>=%.2f' %(hsig.GetMean(), hbkg.GetMean())

        xlep = ROOT.TLatex(0.35, 0.96, text)
        xlep.SetNDC()
        xlep.SetTextFont(42)
        xlep.SetTextSize(0.05)
        xlep.SetTextAlign(12)
        xlep.Draw()        

    #
    # Make ratios
    #
    if options.do_ratio and hdata and hbkg:

        stack.GetXaxis().SetLabelSize(0.0)

        pad = can.cd(2)
    
        hrat = hdata.Clone('%s_ratio' %hdata.GetName())
        hrat.SetDirectory(0)
        hrat.SetStats(False)
        hrat.GetXaxis().SetLabelSize(hbkg.GetXaxis().GetLabelSize())
        hrat.GetYaxis().SetTitle('Ratio')
        hrat.GetYaxis().SetTitleSize(hrat.GetYaxis().GetTitleSize()*1.6)
        hrat.GetYaxis().SetTitleOffset(0.5)
        hrat.GetYaxis().CenterTitle()

        hrat.Divide(hbkg)
        hrat.Draw('')

        f1 = ROOT.TF1('fa1','1', hrat.GetXaxis().GetXmin(), hrat.GetXaxis().GetXmax());
        f1.SetLineColor(4)
        f1.Draw('same');
        
        (rmin, rmax) = getHistMinMax(hrat, True)

        if True:
            hrat.GetYaxis().SetRangeUser(0.1, 2.6)
        else: 
            hrat.GetYaxis().SetRangeUser(rmin, rmax)

        hrat.GetYaxis().SetLabelSize(hrat.GetYaxis().GetLabelSize()*1.6)
        hrat.GetXaxis().SetLabelSize(hrat.GetXaxis().GetLabelSize()*1.6)
        hrat.GetXaxis().SetTitleSize(hrat.GetXaxis().GetTitleSize()*1.6)

    can.cd()
    updateCanvas(can, name=hname)

    return out_hists + [hsig, hbkg, hdata]

#================================================================================
def GetText(channel_name):
        
    if options.do_ratio:
        m_atlasX = 0.75
        m_atlasY = 0.79
        m_atlasSize = 0.05
        deltaX = 1.5*m_atlasSize
        
        Text0 = ROOT.TLatex(m_atlasX-0.08, m_atlasY+0.06, channel_name+', #sqrt{S}=13TeV')
        Text0.SetNDC()
        Text0.SetTextFont(42)
        Text0.SetTextSize(0.9*m_atlasSize)
        
        Text1 = ROOT.TLatex(m_atlasX, m_atlasY-0.01, 'ATLAS')
        Text1.SetNDC()
        Text1.SetTextFont(72)
        Text1.SetTextSize(1.3*m_atlasSize)
        
        Text2 = ROOT.TLatex(m_atlasX+deltaX+0.02, m_atlasY-0.01, 'Internal')
        Text2.SetNDC()
        Text2.SetTextFont(42)
        Text2.SetTextSize(0.9*m_atlasSize)
        
        Text3 = ROOT.TLatex(m_atlasX+0.03, m_atlasY-0.11, '#int Ldt = %i pb^{-1}'%round(options.ilumi))
        Text3.SetNDC()
        Text3.SetTextFont(42)
        Text3.SetTextSize(0.9*m_atlasSize)
    else:
        m_atlasX = 0.60
        m_atlasY = 0.81
        m_atlasSize = 0.04
        deltaX = 1.7*m_atlasSize
        
        Text0 = ROOT.TLatex(m_atlasX-0.03, m_atlasY+0.06, channel_name+', #sqrt{S}=13TeV')
        Text0.SetNDC()
        Text0.SetTextFont(42)
        Text0.SetTextSize(0.9*m_atlasSize)
        
        Text1 = ROOT.TLatex(m_atlasX-0.01, m_atlasY+0.00, 'ATLAS')
        Text1.SetNDC()
        Text1.SetTextFont(72)
        Text1.SetTextSize(1.3*m_atlasSize)
        
        Text2 = ROOT.TLatex(m_atlasX+deltaX+0.03, m_atlasY-0.00, 'Internal')
        Text2.SetNDC()
        Text2.SetTextFont(42)
        Text2.SetTextSize(0.9*m_atlasSize)
        
        Text3 = ROOT.TLatex(m_atlasX+0.04, m_atlasY-0.08, '#int Ldt = %i pb^{-1}'%options.ilumi)
        Text3.SetNDC()
        Text3.SetTextFont(42)
        Text3.SetTextSize(0.9*m_atlasSize)
        
    return [Text0,Text1,Text2,Text3]

#================================================================================
def plotDir(rfile, dfile, can, sman, sig_samples, bkg_samples, oth_samples, dpath):

    samples = sig_samples + bkg_samples + oth_samples

    vetos = ['CutTrack']

    log.debug('plotDir - process %d sample(s)' %len(samples))
    
    for s in samples:
        log.debug('   %-20s %s' %(s.name, s.title))

    sruns = []
    hists = set()

    for sample in samples:
        sruns += sample.GetRuns()

    log.info('plotDir - will use %d run(s)' %(len(sruns)))

    for srun in sruns:
        dir_path = '%s/%s' %(srun, dpath)
        dir_ptr  = rfile.Get(dir_path)

        if not dir_ptr:
            if options.debug:
                log.warning('plotDir - invalid ROOT dir: %s' %(dir_path))
            continue

        for dir_key in dir_ptr.GetListOfKeys():
            nm_func = getattr(dir_key, 'GetName',      None)
            cl_func = getattr(dir_key, 'GetClassName', None)            

            if nm_func and cl_func and cl_func().count('TH1') == 1:
                hists.add(nm_func())

    #
    # Filter and plot histograms
    #
    phists = []
    ohists = []    

    for h in hists:
        if h in vetos:
            continue

        if options.hkey != None and re.search(options.hkey, h) == None:                
            continue

        phists.append(h)

    log.info('plotDir - %3d histogram(s) at: %s' %(len(phists), dpath))
                
    for hist in sorted(phists):
        ohists += plotHist(rfile, dfile, can, sman, sig_samples, bkg_samples, oth_samples, dpath, hist)

    if options.add_runs:
        ohists += getRunHists(sman, rfile, sig_samples+bkg_samples+oth_samples, dpath)

    return ohists

#================================================================================
def getListOfRegions(rfile, options):

    dkeys = set()
    
    for tkey in rfile.GetListOfKeys():    
        
        if len(tkey.GetName()) != 6:
            log.debug('getListOfRegions - skip non TDirectory of wrong size: %s' %tkey.GetName())
            continue

        try:
            int(tkey.GetName())
        except ValueError:
            log.debug('getListOfRegions - skip non-DSID TDirectory: %s' %tkey.GetName())
            continue
            
        dsid = tkey.GetName()
        rdir = rfile.Get(str(dsid))
    
        if not rdir:
            log.warning('getListOfRegions - missing dir: %s' %dsid)
            continue

        if not hasattr(rdir, 'GetListOfKeys'):
            log.warning('getListOfRegions - skip non TDirectory: %s' %dsid)
            continue

        for dkey in rdir.GetListOfKeys():    
            if dkey.GetClassName().count('TDirectory'):
                dkeys.add(dkey.GetName())

                if options.get_subdirs:
                    subdir = rdir.Get(dkey.GetName())
                
                    if subdir and hasattr(subdir, 'GetListOfKeys'):
                        for subdir_key in subdir.GetListOfKeys():
                            if subdir_key.GetClassName().count('TDirectory'):
                                dkeys.add('%s/%s' %(dkey.GetName(), subdir_key.GetName()))
                                
    dkeys = filterList(sorted(list(dkeys)), options.rkey, options.region)

    log.info('getListOfRegions - found %d region(s)' %len(dkeys))

    for d in dkeys:
        log.info('   %s' %d)

    return dkeys


#================================================================================
def readOpt(fpath, key, default=None, do_list=False):

    cfile = open(fpath, 'r')
    results = default

    for line in cfile.readlines():
        parts = line.strip('\n').partition(':')
        
        if parts[0].strip() == key:
            if do_list:
                olist = []
                for p in parts[2].strip().split(','):
                    if len(p.strip()) > 0:
                        olist.append(p.strip())
                    
                results = olist
            else:
                results = parts[2].strip()
            break

    log.info('readOpt - %s: %s' %(key, results))

    return results

#================================================================================
def plotObj(rpath, can):
    
    if not os.path.isfile(rpath):
        log.warning('plotObj - invalid path: %s' %rpath)
        return

    rfile = ROOT.TFile(rpath, 'READ')

    cands = ['prepCand_2lep', 'prepCand_2lep_tau', 'prepCand_3lep']
    objs  = ['CutElec', 'CutMuon', 'CutJet', 'CutTau', 'CutJetBTag']
   
    for cand in cands:
        for obj in objs:
            hpath = '%s/%s' %(cand, obj)
            hname = hpath.replace('/', '_')
            ohist = rfile.Get(hpath)

            if not ohist:
                log.info('plotObj - missing hist: %s' %hpath)
                continue

            ohist.SetStats(False)
            ohist.Draw()
            updateCanvas(can, name=hname)
            
            col = OneCol(obj, obj, ohist, no_err=True)
            tex = prepTable([col], col)
            
            print tex
            
            ofile = open(getOutName('%s.tex' %hname), 'w')
            ofile.write(tex)
            ofile.close()
            
#================================================================================
if __name__ == '__main__':

    #
    # Check inputs
    #
    if len(args) < 1: 
        raise Exception('main - need at least one argument: %s' %(args))

    if len(args) == 1 and not os.path.isfile(args[0]):
        log.error('Invalid input file: %s' %args[0])
        sys.exit(1)

    if not options.xsec_list or not os.path.isfile(options.xsec_list):
        log.error('Need valid parameter: --xsec-list')
        sys.exit(1)

    if options.outdir != None and not os.path.isdir(options.outdir):
        os.makedirs(options.outdir)

    if options.file_dsid and os.path.isfile(getOutName(options.file_dsid)):
        os.remove(getOutName(options.file_dsid))

    if options.file_sample and os.path.isfile(getOutName(options.file_sample)):
        os.remove(getOutName(options.file_sample))

    Config.setPlotDefaults(ROOT)
    can = makeCanvas('can')    
        
    #
    # Open file with histograms and initial MC event normalization info (Counts)
    #
    rfile = XSec.InputFileCollection(args)

    if options.data_file and os.path.isfile(options.data_file):
        log.info('Data input file: %s' %options.data_file)

        dfile = ROOT.TFile(options.data_file, 'READ')
    else:
        dfile = rfile

    #
    # Prepare regions for processing
    #
    regions = []
    subdirs = ['']

    if options.obj_file:
        plotObj(options.obj_file, can)
        sys.exit(0)

    if options.regions:
        regions = options.regions.split(',')

    if options.get_regions:
        regions = getListOfRegions(rfile, options)    
    
    if options.sub_regions:
        subdirs += options.sub_regions.split(',')
    elif options.do_jet:
        subdirs += ['plotJet']
    elif options.do_lep:
        subdirs += ['plotLep_all']
    elif options.do_tau:
        subdirs += ['plotTau']

    #
    # Prepare helper objects for MC normalization
    #
    xman = XSec.XSecMan(options.xsec_list, rfile, options.ilumi, dirname=options.counts_dir)
    xman.bin_norm  = 2
    xman.bin_total = 3
    xman.PrintXSecs()


    bkg_keys = ['ttbar', 'stop2', 'ttw', 'ttz', 'tth2', 'zjets2', 'wjets2', 'vv']
    sig_keys = []
    oth_keys = []

    if options.config_path:
        bkg_keys = readOpt(options.config_path, 'bkg_keys', do_list=True)
        sig_keys = readOpt(options.config_path, 'sig_keys', do_list=True)
        oth_keys = readOpt(options.config_path, 'oth_keys', do_list=True)

    bkg_samples = ttH.getttHSamples(bkg_keys)
    sig_samples = ttH.getttHSamples(sig_keys)
    oth_samples = ttH.getttHSamples(oth_keys)

    if options.scale_fakes:
        for b in bkg_samples:
            if b.name in ['tops', 'vjet']:
                b.SetScaleFactor(options.scale_fakes)

    sman = XSec.SampleMan(sig_samples + bkg_samples + oth_samples, xman, debug=False, no_weight=options.no_xsec_weight)
    sman.PrintSamples()

    if options.save:
        sfile = open(getOutName('sample_list.tex'), 'w')
        sfile.write(sman.GetLatexTable())
        sfile.close()        

        sfile = open(getOutName('sample_miss.tex'), 'w')
        sfile.write(sman.GetLatexTableMiss())
        sfile.close()  

    #
    # Begin Make Object CutFlow Tables:
    #
    if options.obj_cutflow:
        PrepCands = []
        ObjCutflows = ['CutElec', 'CutMuon', 'CutTau', 'CutJet']
        
        for PrepCand in PrepCands:
            for ObjCutHist in ObjCutflows:
                makeObjTable(rfile, sman, sig_samples + bkg_samples, PrepCand, ObjCutHist)
    
    #
    # Plot regions
    # 
    ohists = []
    icount = 0
    dsids  = []

    for region in filterList(regions, options.rkey, options.region):
        icount += 1
        log.info('------------------------------------------------------------------------')
        log.info('Region %d: %s' %(icount, region))

        if options.file_dsid:
            dsids += makeSampleTable(rfile, dfile, sman, sig_samples+bkg_samples+oth_samples, region, 'CutCand')

        makeTable(rfile, dfile, sman, sig_samples, bkg_samples, oth_samples, region, 'CutCand')

        if options.no_plots:
            continue

        for subdir in subdirs:
            if len(subdir) == 0:
                dpath = '%s' %(region)
            else:
                dpath = '%s/%s' %(region, subdir)

            if options.dkey != None and re.search(options.dkey, dpath) == None:
                continue

            log.info(dpath)        
            
            ohists += plotDir(rfile, dfile, can, sman, sig_samples, bkg_samples, oth_samples, dpath)

        if options.nregion and (icount % options.nregion) == 0:
            break

    log.info('Processed %d region(s)' %icount)

    appendLine(getOutName(options.file_dsid), 'xsec_path: %s' %options.xsec_list)
    appendLine(getOutName(options.file_dsid), 'data_path: %s' %options.data_file)
    appendLine(getOutName(options.file_dsid), 'siml_path: %s' %args[0])
    appendLine(getOutName(options.file_dsid), 'dsid_list: %s' %(' '.join(sorted(list(set(dsids))))))

    #
    # Save histograms for individual components and all processed regions
    #
    if options.outfile:

        if options.outdir:
            opath = '%s/%s' %(options.outdir.rstrip('/'), options.outfile)
        else:
            opath = options.outfile

        log.info('Will save %d histogram(s) to %s' %(len(ohists), opath))
        
        ofile = ROOT.TFile(opath, 'RECREATE')

        for ohist in ohists:
            if ohist:
                ohist.SetDirectory(ofile)

        log.info('Will write and close file: %s' %opath)

        ofile.Write()
        ofile.Close()

    log.info('All is done')
