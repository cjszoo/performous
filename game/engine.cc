#include "engine.hh"

const double Engine::TIMESTEP = 0.01;

void Player::update() {
	Tone const* t = m_analyzer.findTone();
	if (t) {
		m_activitytimer = 1000;
		Song const& s = CScreenManager::getSingletonPtr()->getSongs()->current(); // XXX: Kill ScreenManager
		m_scoreIt = s.notes.begin(); // TODO: optimize
		m_pitch.push_back(std::make_pair(t->freq, t->stabledb));
		double beginTime = Engine::TIMESTEP * (m_pitch.size() - 1);
		double endTime = beginTime + 0.01;
		while (m_scoreIt != s.notes.end()) {
			m_score += s.m_scoreFactor * m_scoreIt->score(s.scale.getNote(t->freq), beginTime, endTime);
			if (endTime < m_scoreIt->end) break;
			++m_scoreIt;
		}
		m_score = std::min(1.0, std::max(0.0, m_score));
	} else {
		if (m_activitytimer > 0) --m_activitytimer;
		m_pitch.push_back(std::make_pair(std::numeric_limits<double>::quiet_NaN(), -std::numeric_limits<double>::infinity()));
	}
}
