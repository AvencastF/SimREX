//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar 14 17:58:18 2024 by ROOT version 6.31/01
// from TTree sim/SimREX Event Tree
// found on file: sim.root.0
//////////////////////////////////////////////////////////

#ifndef sim_h
#define sim_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "global-event-model/event.h"

class sim {
public :
    TTree *fChain; //!pointer to the analyzed TTree or TChain
    Int_t fCurrent{}; //!current Tree number in a TChain

    // Fixed size dimensions of array or collections stored in the TTree if any.

    // Declaration of leaf types
    Int_t run_number{};
    Int_t event_number{};
    SimREX::GEM::event *event{};

    // List of branches
    TBranch *b_run_number{}; //!
    TBranch *b_event_number{}; //!
    TBranch *b_event{}; //!

    explicit sim(TTree *tree = nullptr);

    void Init(TTree *tree);

    virtual ~sim();

    virtual Int_t Cut(Long64_t entry);

    virtual Int_t GetEntry(Long64_t entry);

    virtual Long64_t LoadTree(Long64_t entry);

    virtual void Loop();

    virtual bool Notify();

    virtual void Show(Long64_t entry = -1);
};

#endif

#ifdef sim_cxx

sim::sim(TTree *tree) : fChain(nullptr) {
    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.
    if (tree == nullptr) {
        auto *f = dynamic_cast<TFile*>(gROOT->GetListOfFiles()->FindObject("sim.root.0"));
        if (!f || !f->IsOpen()) {
            f = new TFile("sim.root.0");
        }
        f->GetObject("sim", tree);
    }
    Init(tree);
}

sim::~sim() {
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

Int_t sim::GetEntry(Long64_t entry) {
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}

Long64_t sim::LoadTree(Long64_t entry) {
    // Set the environment to read one entry
    if (!fChain) return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent) {
        fCurrent = fChain->GetTreeNumber();
        Notify();
    }
    return centry;
}

void sim::Init(TTree *tree) {
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set object pointer
    event = nullptr;
    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("run_number", &run_number, &b_run_number);
    fChain->SetBranchAddress("event_number", &event_number, &b_event_number);
    fChain->SetBranchAddress("event", &event, &b_event);
    Notify();
}

bool sim::Notify() {
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return true;
}

void sim::Show(Long64_t entry) {
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}

Int_t sim::Cut(Long64_t entry) {
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}

#endif // #ifdef sim_cxx
