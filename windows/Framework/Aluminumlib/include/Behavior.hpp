#ifndef INCLUDE_ALU_BEHAVIOR_HPP
#define INCLUDE_ALU_BEHAVIOR_HPP

#if defined(_WIN32)
#include "Includes.hpp"
#elif defined(__APPLE__)
#include "Aluminum/Includes.hpp"
#endif


#undef IN
#undef OUT

namespace aluminum {

	using glm::dvec3;

	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using glm::mat4;

	class Easing {
	public:
		enum EASE {
			IN, OUT, IN_OUT, OUT_IN
		};
		EASE ease;

		virtual double in(double perc) = 0;

		virtual double out(double perc) = 0;

		virtual double inout(double perc) = 0;

		virtual double outin(double perc) = 0;

		Easing(EASE _ease) {
			ease = _ease;
		}

		double power(double perc, int exponent) {
			double tmp = perc;
			for (int i = 1; i < exponent; i++) {
				tmp *= perc;
			}
			return tmp;
		}

		double easeFunc(double perc) {
			switch (ease) {
			case IN:
				return in(perc);
				break;
			case OUT:
				return out(perc);
				break;
			case IN_OUT:
				return inout(perc);
				break;
			case OUT_IN:
				return outin(perc);
				break;
			default:
				return perc;
				break;
			}
		}
	};

	class EasingPolynomial : public Easing {
	public:
		int exponent;

		EasingPolynomial(Easing::EASE _ease, int _power) : Easing(_ease) {
			exponent = _power;
		}

		virtual double in(double perc) {
			return power(perc, exponent);
		}

		virtual double out(double perc) {
			return 1.0 - power(1.0 - perc, exponent);
		}

		virtual double inout(double perc) {
			double p = perc / 0.5;
			if (p < 1.0) { return .5 * power(p, exponent); }
			else {
				p -= 2.0;
				if (exponent % 2 == 0) { return -.5 * (power(p, exponent) - 2); }
				else { return .5 * (power(p, exponent) + 2); }
			}
		}

		virtual double outin(double perc) { /*haven't done this!*/ return inout(perc);
		}
	};

	class EasingSine : public Easing {
	public:
		EasingSine() : EasingSine(IN) {
		}

		EasingSine(Easing::EASE _ease) : Easing(_ease) {
		}

		virtual double in(double perc) {
			return -cos(perc * (M_PI * .5)) + 1.0;
		}

		virtual double out(double perc) {
			return sin(perc * M_PI * .5);
		}

		virtual double inout(double perc) {
			return (-.5 * (cos(M_PI * perc) - 1));
		}

		virtual double outin(double perc) {
			if (perc < .5f) { return (.5 * sin(M_PI * perc)); }
			else { return 1.0 - (.5 * sin(M_PI * perc)); }
		}
	};


	class EasingBounce : public Easing {
	public:
		int bounce;
		double decayW, decayH, curveWidth;

		EasingBounce(Easing::EASE _ease) : Easing(_ease) {
			initBounce(15, .7f, .5f);
		}

		EasingBounce(Easing::EASE _ease, int _bounce, double _decayW, double _decayH) : Easing(_ease) {
			initBounce(_bounce, _decayW, _decayH);
		}

		void initBounce(int _bounce, double _decayW, double _decayH) {
			bounce = _bounce;
			decayW = _decayW;
			decayH = _decayH;
			double coeff = 0.5;
			for (int i = 1; i < bounce; i++) { coeff += power(decayW, i); }
			curveWidth = 1.0 / coeff;
		}

		virtual double in(double perc) {
			return 1.0 - out(1.0 - perc);
		}

		virtual double out(double perc) {
			float place = curveWidth * .5;
			float prevPlace = place;
			float sp = 1.0;
			float width = place;

			if (perc <= place) { sp = (float)sin((-M_PI / 2.0) + perc * 1.0 / width * (M_PI / 2.0)) + 1.0; }
			else {
				for (int i = 1; i < bounce; i++) {
					width = power(decayW, i) * curveWidth;
					place = prevPlace + width;

					if (perc >= prevPlace && perc < place) {
						sp = sin(-M_PI + (perc - prevPlace) * 1.0 / width * M_PI) + 1.0;
						sp = 1.0 - ((1.0 - sp) * power(decayH, i));
					}

					prevPlace = place;
				}
			}

			return sp;
		}

		virtual double inout(double perc) {
			if (perc < .5) { return .5 * out(perc * 2.0); }
			else { return .5 + .5 * in((perc - .5) * 2.0); }
		}

		virtual double outin(double perc) {
			if (perc < .5) { return .5 * in(perc * 2.0); }
			else { return .5 + .5 * out((perc - .5) * 2.0); }
		}
	};


	class EasingElastic : public EasingBounce {
	public:
		EasingElastic() : EasingBounce(Easing::EASE::OUT, 15, 0.5, 0.5) {
		}

		EasingElastic(int _bounce, double _decay) : EasingBounce(Easing::EASE::OUT, _bounce, _decay, _decay) {
		}

		virtual double out(double perc) {
			double place = curveWidth * 0.5;
			double prevPlace = place;
			double sp = 1.0;
			double width = place;

			if (perc <= place) {
				sp = sin((-M_PI / 2.0) + perc * 1.0 / width * (M_PI / 2.0)) + 1.0;
			}
			else {
				for (int i = 1; i < bounce; i++) {
					width = power(decayW, i) * curveWidth;

					place = prevPlace + width;

					if (perc >= prevPlace && perc < place) {
						if (i % 2 == 0) {
							sp = sin(-M_PI + (perc - prevPlace) * 1.0 / width * M_PI) + 1.0;
						}
						else {
							sp = sin(M_PI - (perc - prevPlace) * 1.0 / width * M_PI) + 1.0;
						}

						sp = 1.0 - ((1.0 - sp) * power(decayH, i));
					}

					prevPlace = place;

				}
			}
			return sp;
		}
	};

	class Behavior {

	public:

		Behavior();

		~Behavior();

		//builder
		Behavior(long startTime);

		Behavior &looping(bool _is);

		Behavior &reversing(bool _is);

		Behavior &repeats(int _n);

		Behavior &length(long _ms);

		Behavior &delay(long _ms);

		Behavior &range(vec3 _r);

		Behavior &range(float _r);

		//easing
		Behavior &sine();

		Behavior &sine(Easing::EASE _e);

		Behavior &polynomial(Easing::EASE _e, int exponent);

		Behavior &bounce(Easing::EASE _e);

		Behavior &bounce(Easing::EASE _e, int bounce, double dW, double dH);

		Behavior &elastic();

		Behavior &elastic(int bounce, double decay);

		Behavior &linear();

		//update
		Behavior &tick(long _tick);

		//get values
		float offset();

		vec3 offsets();

		float total();

		vec3 totals();

		//control
		bool isScheduled;
		bool isActive;
		bool isDone;


	protected:
		void init();

		double calculateOffsetPercentage(double perc, double prevPerc, int dir);

		double getRawPercentage(long currentNano, long loopNano);

		long loopLength;
		double percentage;
		double prevPercentage;

		double offsetPercentage;
		long overshoot;
		vec3 ranges;

		void loopBehavior();

		void reverseBehavior();
		//bool timeToLoop;

		Easing *m_easing;

		vec3 m_offsets;
		vec3 m_totals;

		int repeat;
		int numRepeats;
		int dir;

		long nextTime;

		bool isEasing;
		bool isLooping;
		bool isReversing;

	private:
	};
}

#endif



