/**
 * @file PapiMapEvents.h
 * @brief Header file for PAPI map from string to event.
 * @author Jose M. Abuin
 */
#ifndef PROCESS_MONITOR_PAPIMAPEVENTS_H
#define PROCESS_MONITOR_PAPIMAPEVENTS_H

#include <unordered_map>
#include <string>

#ifdef __cplusplus
extern "C"
{
#endif
#include "third-party/papi/papi.h"
#ifdef __cplusplus
}
#endif

/// @brief Namespace storing info for Papi events mapping.
namespace PapiMapEvents {

    /// @brief Map from string to Papi event.
    const std::unordered_map<std::string, int> PapiMapEvents = {
            {"PAPI_L1_DCM", PAPI_L1_DCM},
            {"PAPI_L1_ICM", PAPI_L1_ICM},
            {"PAPI_L2_DCM", PAPI_L2_DCM},
            {"PAPI_L2_ICM", PAPI_L2_ICM},
            {"PAPI_L3_DCM", PAPI_L3_DCM},
            {"PAPI_L3_ICM", PAPI_L3_ICM},
            {"PAPI_L1_TCM", PAPI_L1_TCM},
            {"PAPI_L2_TCM", PAPI_L2_TCM},
            {"PAPI_L3_TCM", PAPI_L3_TCM},
            {"PAPI_CA_SNP", PAPI_CA_SNP},
            {"PAPI_CA_SHR", PAPI_CA_SHR},
            {"PAPI_CA_CLN", PAPI_CA_CLN},
            {"PAPI_CA_INV", PAPI_CA_INV},
            {"PAPI_CA_ITV", PAPI_CA_ITV},
            {"PAPI_L3_LDM", PAPI_L3_LDM},
            {"PAPI_L3_STM", PAPI_L3_STM},

            {"PAPI_BRU_IDL", PAPI_BRU_IDL},
            {"PAPI_FXU_IDL", PAPI_FXU_IDL},
            {"PAPI_FPU_IDL", PAPI_FPU_IDL},
            {"PAPI_LSU_IDL", PAPI_LSU_IDL},
            {"PAPI_TLB_DM", PAPI_TLB_DM},
            {"PAPI_TLB_IM", PAPI_TLB_IM},
            {"PAPI_TLB_TL", PAPI_TLB_TL},
            {"PAPI_L1_LDM", PAPI_L1_LDM},
            {"PAPI_L1_STM", PAPI_L1_STM},
            {"PAPI_L2_LDM", PAPI_L2_LDM},
            {"PAPI_L2_STM", PAPI_L2_STM},
            {"PAPI_BTAC_M", PAPI_BTAC_M},
            {"PAPI_PRF_DM", PAPI_PRF_DM},
            {"PAPI_L3_DCH", PAPI_L3_DCH},
            {"PAPI_TLB_SD", PAPI_TLB_SD},
            {"PAPI_CSR_FAL", PAPI_CSR_FAL},

            {"PAPI_CSR_SUC", PAPI_CSR_SUC},
            {"PAPI_CSR_TOT", PAPI_CSR_TOT},
            {"PAPI_MEM_SCY", PAPI_MEM_SCY},
            {"PAPI_MEM_RCY", PAPI_MEM_RCY},
            {"PAPI_MEM_WCY", PAPI_MEM_WCY},
            {"PAPI_STL_ICY", PAPI_STL_ICY},
            {"PAPI_FUL_ICY", PAPI_FUL_ICY},
            {"PAPI_STL_CCY", PAPI_STL_CCY},
            {"PAPI_FUL_CCY", PAPI_FUL_CCY},
            {"PAPI_HW_INT", PAPI_HW_INT},
            {"PAPI_BR_UCN", PAPI_BR_UCN},
            {"PAPI_BR_CN", PAPI_BR_CN},
            {"PAPI_BR_TKN", PAPI_BR_TKN},
            {"PAPI_BR_NTK", PAPI_BR_NTK},
            {"PAPI_BR_MSP", PAPI_BR_MSP},
            {"PAPI_BR_PRC", PAPI_BR_PRC},

            {"PAPI_FMA_INS", PAPI_FMA_INS},
            {"PAPI_TOT_IIS", PAPI_TOT_IIS},
            {"PAPI_TOT_INS", PAPI_TOT_INS},
            {"PAPI_INT_INS", PAPI_INT_INS},
            {"PAPI_FP_INS", PAPI_FP_INS},
            {"PAPI_LD_INS", PAPI_LD_INS},
            {"PAPI_SR_INS", PAPI_SR_INS},
            {"PAPI_BR_INS", PAPI_BR_INS},
            {"PAPI_VEC_INS", PAPI_VEC_INS},
            {"PAPI_RES_STL", PAPI_RES_STL},
            {"PAPI_FP_STAL", PAPI_FP_STAL},
            {"PAPI_TOT_CYC", PAPI_TOT_CYC},
            {"PAPI_LST_INS", PAPI_LST_INS},
            {"PAPI_SYC_INS", PAPI_SYC_INS},
            {"PAPI_L1_DCH", PAPI_L1_DCH},
            {"PAPI_L2_DCH", PAPI_L2_DCH},

            {"PAPI_L1_DCA", PAPI_L1_DCA},
            {"PAPI_L2_DCA", PAPI_L2_DCA},
            {"PAPI_L3_DCA", PAPI_L3_DCA},
            {"PAPI_L1_DCR", PAPI_L1_DCR},
            {"PAPI_L2_DCR", PAPI_L2_DCR},
            {"PAPI_L3_DCR", PAPI_L3_DCR},
            {"PAPI_L1_DCW", PAPI_L1_DCW},
            {"PAPI_L2_DCW", PAPI_L2_DCW},
            {"PAPI_L3_DCW", PAPI_L3_DCW},
            {"PAPI_L1_ICH", PAPI_L1_ICH},
            {"PAPI_L2_ICH", PAPI_L2_ICH},
            {"PAPI_L3_ICH", PAPI_L3_ICH},
            {"PAPI_L1_ICA", PAPI_L1_ICA},
            {"PAPI_L2_ICA", PAPI_L2_ICA},
            {"PAPI_L3_ICA", PAPI_L3_ICA},
            {"PAPI_L1_ICR", PAPI_L1_ICR},

            {"PAPI_L2_ICR", PAPI_L2_ICR},
            {"PAPI_L3_ICR", PAPI_L3_ICR},
            {"PAPI_L1_ICW", PAPI_L1_ICW},
            {"PAPI_L2_ICW", PAPI_L2_ICW},
            {"PAPI_L3_ICW", PAPI_L3_ICW},
            {"PAPI_L1_TCH", PAPI_L1_TCH},
            {"PAPI_L2_TCH", PAPI_L2_TCH},
            {"PAPI_L3_TCH", PAPI_L3_TCH},
            {"PAPI_L1_TCA", PAPI_L1_TCA},
            {"PAPI_L2_TCA", PAPI_L2_TCA},
            {"PAPI_L3_TCA", PAPI_L3_TCA},
            {"PAPI_L1_TCR", PAPI_L1_TCR},
            {"PAPI_L2_TCR", PAPI_L2_TCR},
            {"PAPI_L3_TCR", PAPI_L3_TCR},
            {"PAPI_L1_TCW", PAPI_L1_TCW},
            {"PAPI_L2_TCW", PAPI_L2_TCW},

            {"PAPI_L3_TCW", PAPI_L3_TCW},
            {"PAPI_FML_INS", PAPI_FML_INS},
            {"PAPI_FAD_INS", PAPI_FAD_INS},
            {"PAPI_FDV_INS", PAPI_FDV_INS},
            {"PAPI_FSQ_INS", PAPI_FSQ_INS},
            {"PAPI_FNV_INS", PAPI_FNV_INS},
            {"PAPI_FP_OPS", PAPI_FP_OPS},
            {"PAPI_SP_OPS", PAPI_SP_OPS},
            {"PAPI_DP_OPS", PAPI_DP_OPS},
            {"PAPI_VEC_SP", PAPI_VEC_SP},
            {"PAPI_VEC_DP", PAPI_VEC_DP},
            {"PAPI_REF_CYC", PAPI_REF_CYC},
            {"PAPI_END", PAPI_END},

    };
}


#endif //PROCESS_MONITOR_PAPIMAPEVENTS_H
