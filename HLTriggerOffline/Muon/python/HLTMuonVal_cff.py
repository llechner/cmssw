import FWCore.ParameterSet.Config as cms

from Validation.RecoMuon.muonValidationHLT_cff import *
from HLTriggerOffline.Muon.muonTriggerRateTimeAnalyzer_cfi import *
from Validation.RecoMuon.muonValidationHLTFastSim_cff import *

print "Initializing muonBits"
from DQM.HLTEvF.HLTMonMuonBits_cfi import *
relvalMuonBits = hltMonMuBits.clone(
    directory = cms.untracked.string('HLT/Muon/'),
    HLTPaths = cms.vstring('HLT_L1Mu+',
                           'HLT_L2Mu+',
                           'HLT_Mu+',
                           'HLT_IsoMu+',
                           'HLT_DoubleMu+'
                           )
    )

HLTMuonVal = cms.Sequence(
    recoMuonValidationHLT_seq + 
    muonTriggerRateTimeAnalyzer +
    relvalMuonBits
    )

HLTMuonVal_FastSim = cms.Sequence(
    recoMuonValidationHLTFastSim_seq +
    muonTriggerRateTimeAnalyzer +
    relvalMuonBits
    )

