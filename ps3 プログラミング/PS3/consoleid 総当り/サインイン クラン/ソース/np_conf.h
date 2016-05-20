/*  SCE CONFIDENTIAL
 *  PlayStation(R)3 Programmer Tool Runtime Library 400.001
 *  Copyright (C) 2009 Sony Computer Entertainment Inc.
 *  All Rights Reserved.
 */

#include <np.h>

#ifndef __SAMPLE_NP_CONF_H__
#define __SAMPLE_NP_CONF_H__

class NpConf {
private:
	NpConf();
	virtual ~NpConf();

	NpConf(const NpConf&);
	NpConf& operator=(const NpConf&);

public:
	static const SceNpCommunicationId* npCommId(int i = 0);
	static const SceNpCommunicationPassphrase* npCommPassphrase(int i = 0);
	static const SceNpCommunicationSignature* npCommSig(int i = 0);
};

#endif // __SAMPLE_NP_CONF_H__
