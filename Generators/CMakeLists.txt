# Copyright CERN and copyright holders of ALICE O2. This software is distributed
# under the terms of the GNU General Public License v3 (GPL Version 3), copied
# verbatim in the file "COPYING".
#
# See http://alice-o2.web.cern.ch/license for full licensing information.
#
# In applying this license CERN does not waive the privileges and immunities
# granted to it by virtue of its status as an Intergovernmental Organization or
# submit itself to any jurisdiction.

o2_add_library(Generators
               SOURCES src/Generator.cxx
	       	       src/Trigger.cxx
	       	       src/TriggerParticle.cxx
                       src/GeneratorFromFile.cxx
                       src/PDG.cxx
                       src/PrimaryGenerator.cxx
                       src/InteractionDiamondParam.cxx
                       src/TriggerParticleParam.cxx
                       src/BoxGunParam.cxx
		       src/QEDGenParam.cxx
                       src/GeneratorFactory.cxx
               PUBLIC_LINK_LIBRARIES FairRoot::Base O2::SimConfig O2::CommonUtils
                                     O2::SimulationDataFormat
                                     FairRoot::Gen
               TARGETVARNAME targetName)

set(headers
    include/Generators/Generator.h
    include/Generators/Trigger.h
    include/Generators/TriggerParticle.h
    include/Generators/GeneratorFromFile.h
    include/Generators/PDG.h
    include/Generators/PrimaryGenerator.h
    include/Generators/InteractionDiamondParam.h
    include/Generators/TriggerParticleParam.h
    include/Generators/ConfigurationMacroHelper.h
    include/Generators/BoxGunParam.h
    include/Generators/QEDGenParam.h
    include/Generators/GeneratorFactory.h)

o2_target_root_dictionary(Generators HEADERS ${headers})

