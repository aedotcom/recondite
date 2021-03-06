#ifndef R_LOGCHAIN_HPP
#define R_LOGCHAIN_HPP

#include <list>
#include <iterator>

#include "rTypes.hpp"
#include "rDefs.hpp"

#include "rLogTarget.hpp"

class RECONDITE_API rLogChain : public rLogTarget {
public:
	rLogChain(rLogTarget* target);
	rLogChain(rLogTarget* target1, rLogTarget* target2);
	~rLogChain();

public:
	void AddTarget(rLogTarget* target);
	
	void DeleteTarget(size_t index);
	rLogTarget* GetTarget(size_t index) const;
	
	void DeleteAllTargets();
	
	size_t NumTargets() const;
	
	virtual void Debug(const rString& message);
	virtual void Warning(const rString& message);
	virtual void Trace(const rString& message);
	virtual void Info(const rString& message);
	virtual void Error(const rString& message);

	virtual void Shutdown();
	
private:
	typedef std::list<rLogTarget*> rLogTargetList;

private:
	rLogTargetList m_targets;
};

#endif