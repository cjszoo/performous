#pragma once
#ifndef PERFORMOUS_NOTES_HH
#define PERFORMOUS_NOTES_HH

#include <string>
#include <vector>

/// musical scale, defaults to C major
class MusicalScale {
  private:
	double m_baseFreq;
	static const int m_baseId = 33;

  public:
	/// constructor
	MusicalScale(double baseFreq = 440.0): m_baseFreq(baseFreq) {}
	/// get name of note
	std::string getNoteStr(double freq) const;
	/// get note number for id
	unsigned int getNoteNum(int id) const;
	/// true if sharp note
	bool isSharp(int id) const;
	/// get frequence for note id
	double getNoteFreq(int id) const;
	/// get note id for frequence
	int getNoteId(double freq) const;
	/// get note for frequence
	double getNote(double freq) const;
	/// get note offset for frequence
	double getNoteOffset(double freq) const;
};

/// note read from songfile
struct Note {
	double begin, ///< begin time
	       end; ///< end time
	/// power of note
	mutable double power;
	/// note type
	enum Type { FREESTYLE = 'F', NORMAL = ':', GOLDEN = '*', SLEEP = '-'} type;
	/// note
	int note;
	/// lyrics syllable for that note
	std::string syllable;
	/// Difference of n from note
	double diff(double n) const { return diff(note, n); }
	/// Difference of n from note, so that note + diff(note, n) is n (mod 12)
	static double diff(double note, double n);
	/// maximum score
	double maxScore() const;
	/// score when singing
	double score(double freq, double b, double e) const;

  private:
	double scoreMultiplier(double error) const;
};

typedef std::vector<Note> Notes;

#endif