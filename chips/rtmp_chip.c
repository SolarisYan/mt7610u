/*
 *************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36, Taiyuan St., Jhubei City,
 * Hsinchu County 302,
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2010, Ralink Technology, Inc.
 *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program; if not, write to the                         *
 * Free Software Foundation, Inc.,                                       *
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                       *
 *************************************************************************/


#include "rt_config.h"



FREQUENCY_ITEM RtmpFreqItems3020[] =
{
	/* ISM : 2.4 to 2.483 GHz,  11g */
	/*-CH---N-------R---K-----------*/
	{1,    241,  2,  2},
	{2,    241,	 2,  7},
	{3,    242,	 2,  2},
	{4,    242,	 2,  7},
	{5,    243,	 2,  2},
	{6,    243,	 2,  7},
	{7,    244,	 2,  2},
	{8,    244,	 2,  7},
	{9,    245,	 2,  2},
	{10,   245,	 2,  7},
	{11,   246,	 2,  2},
	{12,   246,	 2,  7},
	{13,   247,	 2,  2},
	{14,   248,	 2,  4},
};

FREQUENCY_ITEM FreqItems3020_Xtal20M[] =
{
	/*
	 * RF_R08:
	 * <7:0>: pll_N<7:0>
	 *
	 * RF_R09:
	 * <3:0>: pll_K<3:0>
	 * <4>: pll_N<8>
	 * <7:5>pll_N<11:9>
	 *
	 */
	/*-CH---N--------R---N[7:4]K[3:0]------*/
	{1,    0xE2,     2,  0x14},
	{2,    0xE3,	 2,  0x14},
	{3,    0xE4,	 2,  0x14},
	{4,    0xE5,	 2,  0x14},
	{5,    0xE6,	 2,  0x14},
	{6,    0xE7,	 2,  0x14},
	{7,    0xE8,	 2,  0x14},
	{8,    0xE9,	 2,  0x14},
	{9,    0xEA,	 2,  0x14},
	{10,   0xEB,	 2,  0x14},
	{11,   0xEC,	 2,  0x14},
	{12,   0xED,	 2,  0x14},
	{13,   0xEE,	 2,  0x14},
	{14,   0xF0,	 2,  0x18},
};

u8 NUM_OF_3020_CHNL = (sizeof(RtmpFreqItems3020) / sizeof(FREQUENCY_ITEM));

FREQUENCY_ITEM *FreqItems3020 = RtmpFreqItems3020;

/*
========================================================================
Routine Description:
	Initialize specific beacon frame architecture.

Arguments:
	pAd				- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
void RtmpChipBcnSpecInit(struct rtmp_adapter*pAd)
{
}


/*
========================================================================
Routine Description:
	Initialize normal beacon frame architecture.

Arguments:
	pAd				- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
void RtmpChipBcnInit(
	IN struct rtmp_adapter*pAd)
{
	struct rtmp_chip_cap *pChipCap = &pAd->chipCap;


	pChipCap->FlgIsSupSpecBcnBuf = false;
	pChipCap->BcnMaxHwNum = 8;
	pChipCap->BcnMaxNum = (pChipCap->BcnMaxHwNum - MAX_MESH_NUM - MAX_APCLI_NUM);
	pChipCap->BcnMaxHwSize = 0x1000;

	pChipCap->BcnBase[0] = 0x7800;
	pChipCap->BcnBase[1] = 0x7A00;
	pChipCap->BcnBase[2] = 0x7C00;
	pChipCap->BcnBase[3] = 0x7E00;
	pChipCap->BcnBase[4] = 0x7200;
	pChipCap->BcnBase[5] = 0x7400;
	pChipCap->BcnBase[6] = 0x5DC0;
	pChipCap->BcnBase[7] = 0x5BC0;

	/*
		If the MAX_MBSSID_NUM is larger than 6,
		it shall reserve some WCID space(wcid 222~253) for beacon frames.
		-	these wcid 238~253 are reserved for beacon#6(ra6).
		-	these wcid 222~237 are reserved for beacon#7(ra7).
	*/
	if (pChipCap->BcnMaxNum == 8)
		pChipCap->WcidHwRsvNum = 222;
	else if (pChipCap->BcnMaxNum == 7)
		pChipCap->WcidHwRsvNum = 238;
	else
		pChipCap->WcidHwRsvNum = 255;

	pAd->chipOps.BeaconUpdate = RtmpChipWriteMemory;
}


/*
========================================================================
Routine Description:
	Initialize specific beacon frame architecture.

Arguments:
	pAd				- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
void rlt_bcn_buf_init(struct rtmp_adapter*pAd)
{
	struct rtmp_chip_cap *pChipCap = &pAd->chipCap;

	pChipCap->FlgIsSupSpecBcnBuf = false;
	pChipCap->BcnMaxHwNum = 16;
	pChipCap->WcidHwRsvNum = 255;

/*
	In 16-MBSS support mode, if AP-Client is enabled,
	the last 8-MBSS would be occupied for AP-Client using.
*/
#ifdef APCLI_SUPPORT
	pChipCap->BcnMaxNum = (8 - MAX_MESH_NUM);
#else
	pChipCap->BcnMaxNum = (pChipCap->BcnMaxHwNum - MAX_MESH_NUM);
#endif /* APCLI_SUPPORT */

	pChipCap->BcnMaxHwSize = 0x2000;

	pChipCap->BcnBase[0] = 0xc000;
	pChipCap->BcnBase[1] = 0xc200;
	pChipCap->BcnBase[2] = 0xc400;
	pChipCap->BcnBase[3] = 0xc600;
	pChipCap->BcnBase[4] = 0xc800;
	pChipCap->BcnBase[5] = 0xca00;
	pChipCap->BcnBase[6] = 0xcc00;
	pChipCap->BcnBase[7] = 0xce00;
	pChipCap->BcnBase[8] = 0xd000;
	pChipCap->BcnBase[9] = 0xd200;
	pChipCap->BcnBase[10] = 0xd400;
	pChipCap->BcnBase[11] = 0xd600;
	pChipCap->BcnBase[12] = 0xd800;
	pChipCap->BcnBase[13] = 0xda00;
	pChipCap->BcnBase[14] = 0xdc00;
	pChipCap->BcnBase[15] = 0xde00;

	pAd->chipOps.BeaconUpdate = RtmpChipWriteMemory;

	DBGPRINT(RT_DEBUG_TRACE, ("< Beacon Spec Information: >\n"));
	DBGPRINT(RT_DEBUG_TRACE, ("\tBcnMaxHwNum = \t%d\n", pChipCap->BcnMaxHwNum));
	DBGPRINT(RT_DEBUG_TRACE, ("\tBcnMaxNum = \t%d\n", pChipCap->BcnMaxNum));
	DBGPRINT(RT_DEBUG_TRACE, ("\tBcnMaxHwSize = \t0x%x\n", pChipCap->BcnMaxHwSize));
	DBGPRINT(RT_DEBUG_TRACE, ("\tWcidHwRsvNum = \t%d\n", pChipCap->WcidHwRsvNum));
	DBGPRINT(RT_DEBUG_TRACE, ("\tBcnBase[0] = \t0x%x\n", pChipCap->BcnBase[0]));
	DBGPRINT(RT_DEBUG_TRACE, ("\tBcnBase[8] = \t0x%x\n", pChipCap->BcnBase[8]));

}

/*
========================================================================
Routine Description:
	write memory

Arguments:
	pAd				- WLAN control block pointer
	Offset			- Memory offsets
	Value			- Written value
	Unit				- Unit in "Byte"
Return Value:
	None

Note:
========================================================================
*/
void RtmpChipWriteMemory(
	IN	struct rtmp_adapter*pAd,
	IN	USHORT			Offset,
	IN	u32			Value,
	IN	u8			Unit)
{
	switch(Unit)
	{
		case 1:
			RTMP_IO_WRITE8(pAd, Offset, Value);
			break;
		case 2:
			RTMP_IO_WRITE16(pAd, Offset, Value);
			break;
		case 4:
			mt7610u_write32(pAd, Offset, Value);
		default:
			break;
	}
}




static void RxSensitivityTuning(struct rtmp_adapter*pAd)
{
	u8 R66 = 0x26 + GET_LNA_GAIN(pAd);

	{
		rtmp_bbp_set_agc(pAd, R66, RX_CHAIN_ALL);
	}
	DBGPRINT(RT_DEBUG_TRACE,("turn off R17 tuning, restore to 0x%02x\n", R66));
}


#ifdef CONFIG_STA_SUPPORT
static u8 ChipAGCAdjust(
	IN struct rtmp_adapter *	pAd,
	IN CHAR					Rssi,
	IN u8 			OrigR66Value)
{
	u8 R66 = OrigR66Value;
	CHAR lanGain = GET_LNA_GAIN(pAd);

	if (pAd->LatchRfRegs.Channel <= 14)
	{	/*BG band*/
		R66 = 0x2E + lanGain;
		if (Rssi > RSSI_FOR_MID_LOW_SENSIBILITY)
			R66 += 0x10;
	}
	else
	{	/*A band*/
		if (pAd->CommonCfg.BBPCurrentBW == BW_20)
			R66 = 0x32 + (lanGain * 5) / 3;
		else
			R66 = 0x3A + (lanGain * 5) / 3;

		if (Rssi > RSSI_FOR_MID_LOW_SENSIBILITY)
				R66 += 0x10;
	}

	if (OrigR66Value != R66)
		rtmp_bbp_set_agc(pAd, R66, RX_CHAIN_ALL);


	return R66;
}
#endif /* CONFIG_STA_SUPPORT */

INT WaitForAsicReady(
	IN struct rtmp_adapter*pAd)
{
	u32 mac_val = 0, reg = MAC_CSR0;
	int idx = 0;

	do
	{
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))
			return false;

		mac_val = mt7610u_read32(pAd, reg);
		if ((mac_val != 0x00) && (mac_val != 0xFFFFFFFF))
			return true;

		mdelay(5);
	} while (idx++ < 500);

	DBGPRINT(RT_DEBUG_ERROR,
				("%s(0x%x):AsicNotReady!\n",
				__FUNCTION__, mac_val));


	return false;
}

/*
========================================================================
Routine Description:
	Initialize chip related information.

Arguments:
	pCB				- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
int RtmpChipOpsHook(void *pCB)
{
	struct rtmp_adapter*pAd = (struct rtmp_adapter*)pCB;
	struct rtmp_chip_ops  *pChipOps = &pAd->chipOps;
	struct rtmp_chip_cap *pChipCap = &pAd->chipCap;
	u32 MacValue;
	int ret = 0;


	/* sanity check */
	if (WaitForAsicReady(pAd) == false)
		return -1;

	MacValue = mt7610u_read32(pAd, MAC_CSR0);
	pAd->MACVersion = MacValue;

	if (pAd->MACVersion == 0xffffffff)
		return -1;

#ifdef MT76x0
	if (IS_MT76x0(pAd)) {
		MT76x0_Init(pAd);
		goto done;
	}
#endif /* MT76x0 */

done:
	DBGPRINT(RT_DEBUG_TRACE, ("Chip VCO calibration mode = %d!\n", pChipCap->FlgIsVcoReCalMode));
	return ret;
}
