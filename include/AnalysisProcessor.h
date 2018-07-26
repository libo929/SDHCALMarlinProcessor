#ifndef AnalysisProcessor_h
#define AnalysisProcessor_h

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>
#include <cstring>
#include <EVENT/CalorimeterHit.h>
#include <vector>
#include <map>
#include <limits>
#include <iostream>

#include "CaloObject/CaloGeom.h"
#include "CaloObject/CaloHit.h"
#include "CaloObject/CaloCluster.h"
#include "CaloObject/Asic.h"
#include "CaloObject/Shower.h"
#include "Algorithm/Clustering.h"
#include "Algorithm/Tracking.h"
#include "Algorithm/Hough.h"
#include "Algorithm/ClusteringHelper.h"
#include "Algorithm/InteractionFinder.h"
#include "Algorithm/Efficiency.h"
#include "Algorithm/AsicKeyFinder.h"
#include "Algorithm/ShowerAnalyser.h"
#include "Algorithm/Density.h"

#include <marlin/tinyxml.h>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>

using namespace lcio ;
using namespace marlin ;

enum DifPos { left , center , right , all } ;
std::ostream& operator<<(std::ostream& os , const DifPos& difPos)
{
	if ( difPos == left )
		os << "left" ;
	else if ( difPos == center )
		os << "center" ;
	else if ( difPos == right )
		os << "right" ;
	else
		os << "all" ;
	return os ;
}

class AnalysisProcessor : public Processor
{

	public :

		virtual Processor* newProcessor() { return new AnalysisProcessor ; }


		AnalysisProcessor() ;

		/** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
		virtual void init() ;

		/** Called for every run.
   */
		virtual void processRunHeader(LCRunHeader* ) ;

		/** Called for every event - the working horse.
   */
		virtual void processEvent(LCEvent* evt) ;


		virtual void check(LCEvent* ) ;


		/** Called after data processing for clean up.
   */
		virtual void end() ;

		void AlgorithmRegistrationParameters() ;

		void clearVec() ;

		void findEventTime(LCEvent* evt , LCCollection* _col) ;

		double getFirst5LayersRMS() ;

		AnalysisProcessor(const AnalysisProcessor &toCopy) = delete ;
		void operator=(const AnalysisProcessor &toCopy) = delete ;


	protected :

		const std::map<DifPos , std::pair<int,int>> difLimits = { {left,{1,32}} , {center,{33,64}} , {right,{65,96}} , {all,{1,96}} } ;

		void processRecoverXmlFile() ;
		std::vector<float> recoverHits() const ;
		std::vector<float> ignoreHits() const ;

		std::vector< std::pair<int , DifPos> > recoverList = {} ;
		std::vector< std::pair<int , DifPos> > ignoreList = {} ;


		int _nRun = 0 ;
		int _nEvt = 0 ;
		/** Input collection name.
   */
		std::vector<std::string> _hcalCollections = {} ;

		std::string recoverXmlFile = "" ;

	private :
		std::map<int,HitVec> hitMap ;
		std::vector<caloobject::CaloCluster2D*> clusterVec ;

		/*--------------------Global parameters--------------------*/
		int _nActiveLayers = 0 ;
		int numElements = 0 ;
		LCCollection* col = nullptr ;
		std::vector<float> edges ; //vector to recover geometry parameters
		CLHEP::Hep3Vector posShift ;
		/*------------------------------------------------------------------------------*/

		/*--------------------Algorithms list to initialise--------------------*/
		algorithm::Clustering* algo_Cluster ;
		algorithm::ClusteringHelper* algo_ClusteringHelper ;
		algorithm::Tracking* algo_Tracking ;
		algorithm::Hough* algo_Hough ;
		algorithm::InteractionFinder* algo_InteractionFinder ;
		algorithm::Density* algo_density ;

		/*------------------------------------------------------------------------------*/

		/*--------------------Algorithms setting parameter structure--------------------*/
		algorithm::clusterParameterSetting m_ClusterParameterSetting ;
		algorithm::ClusteringHelperParameterSetting m_ClusteringHelperParameterSetting ;
		algorithm::TrackingParameterSetting m_TrackingParameterSetting ;
		algorithm::HoughParameterSetting m_HoughParameterSetting ;
		algorithm::InteractionFinderParameterSetting m_InteractionFinderParameterSetting ;

		/*------------------------------------------------------------------------------*/

		/*--------------------CaloObject setting parameter structure--------------------*/
		caloobject::GeomParameterSetting m_CaloGeomSetting ;

		/*---------------------------------------------------------------------*/

		std::vector<float> thresholds = {} ;

		double _timeCut = 0 ;
		unsigned long long _prevBCID = 0 ;
		unsigned long long _bcidRef = 0 ;

		unsigned long long firstBCIDOfRun = 0 ;

		int _timeDif_minus_bif = 0 ;

		/*--------------------Root output object--------------------*/
		std::string outputRootName = "" ;
		TFile *file = nullptr ;
		TTree* tree = nullptr ;

		double computingTime = 0 ;

		int eventNumber = 0 ;
		unsigned long long evtTime = 0 ;
		unsigned long long spillEvtTime = 0 ;

		int cerenkovTag = 0 ;

		int runNumber = 0 ;
		float energy = 0 ;

		int nHit = 0 ;
		int nHit1 = 0 ;
		int nHit2 = 0 ;
		int nHit3 = 0 ;
		int nHough = 0 ;
		int nHough1 = 0 ;
		int nHough2 = 0 ;
		int nHough3 = 0 ;
		int nLayer = 0 ;
		int nInteractingLayer = 0 ;
		int nCluster = 0 ;
		int nMipCluster = 0 ;
		int nTrack = 0 ;

		double first5LayersRMS = 0 ;

		std::vector<int> tracksClusterSize ;
		std::vector<int> tracksClusterNumber ;

		double begin = 0 ;
		double _end = 0 ;

		double reconstructedCosTheta = 0 ;

		double density = 0 ;
		float transverseRatio = 0 ;

		float thrust[4] = {0,0,0,0} ;

		double meanRadius = 0 ;

		bool neutral = 0 ;

		std::vector<double> longiProfile = {} ;
		std::vector<double> radiProfile = {} ;

		std::vector<int> iVec = {} ;
		std::vector<int> jVec = {} ;
		std::vector<int> kVec = {} ;
		std::vector<float> thrVec = {} ;
		std::vector<float> timeVec = {} ;
		std::vector<float> densityVec = {} ;

		double emFraction = 0 ;
		double depositedEnergy = 0 ;
		double leakedEnergy = 0 ;

		float nHitCustom = 0 ;
		float nHit1Custom = 0 ;
		float nHit2Custom = 0 ;
		float nHit3Custom = 0 ;
} ;


#endif //AnalysisProcessor_h
