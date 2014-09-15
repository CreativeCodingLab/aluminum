#include "Aluminum/Behavior.hpp"

//#ifndef NANO
//#define NANO 1
//#endif


#ifndef MILLIS
#define MILLIS 1
#endif

namespace aluminum {

    Behavior::Behavior() {
    } //hmm
    Behavior::~Behavior() {
    }

    Behavior::Behavior(long startTime) {
        nextTime = startTime;

        //defaults
        length(10000);
        range(vec3(1.0));
        looping(false);
        reversing(false);
        repeats(0);
        linear();

        init();
    }

    void Behavior::init() {
        m_offsets = dvec3(0, 0, 0);
        m_totals = dvec3(0, 0, 0);

        percentage = 0.0;
        prevPercentage = 0.0;
        offsetPercentage = 0.0;
        overshoot = 0.0;

        isDone = false;
        isActive = false;
        isScheduled = false;

        dir = 1;
        repeat = 0;
    }


    Behavior &Behavior::delay(long _ms) {
#ifdef NANO
        nextTime += _ms * 1000000;
#else 
      nextTime += _ms;
#endif
        return *this;
    }

    Behavior &Behavior::length(long _ms) {
#ifdef NANO
      loopLength = _ms * 1000000;
#else
      loopLength = _ms;

#endif
        return *this;
    }

    Behavior &Behavior::range(vec3 _r) {
        ranges = _r;
        return *this;
    }

    Behavior &Behavior::range(float _r) {
        ranges = vec3(_r, 0.0f, 0.0f);
        return *this;
    }

    Behavior &Behavior::linear() {
        isEasing = false;
        return *this;
    }

    Behavior &Behavior::sine() {
        return sine(Easing::IN);
    }

    Behavior &Behavior::sine(Easing::EASE ease) {
        m_easing = new EasingSine(ease);
        isEasing = true;
        return *this;
    }

    Behavior &Behavior::elastic() {
        m_easing = new EasingElastic();
        isEasing = true;
        return *this;
    }

    Behavior &Behavior::elastic(int bounce, double d) {
        m_easing = new EasingElastic(bounce, d);
        isEasing = true;
        return *this;
    }

    Behavior &Behavior::polynomial(Easing::EASE ease, int power) {
        m_easing = new EasingPolynomial(ease, power);
        isEasing = true;
        return *this;
    }

    Behavior &Behavior::bounce(Easing::EASE ease) {
        m_easing = new EasingBounce(ease);
        isEasing = true;
        return *this;
    }

    Behavior &Behavior::bounce(Easing::EASE ease, int bounce, double dW, double dH) {
        m_easing = new EasingBounce(ease, bounce, dW, dH);
        isEasing = true;
        return *this;
    }

    Behavior &Behavior::repeats(int _n) { //-1 = forever
        numRepeats = _n;
        return *this;
    }

    Behavior &Behavior::looping(bool _is) {
        isLooping = _is;
        return *this;
    }

    Behavior &Behavior::reversing(bool _is) {
        isReversing = _is;
        return *this;
    }

    vec3 Behavior::offsets() {
        return m_offsets;
    }

    float Behavior::offset() {
        return m_offsets.x;
    }

    vec3 Behavior::totals() {
        return m_totals;
    }

    float Behavior::total() {
        return m_totals.x;
    }

    double Behavior::getRawPercentage(long currentNano, long loopNano) {
        // printf("currentNano = %f, loopNano = %f, RawPercentage = %f\n", currentNano, loopNano, (currentNano / loopNano));
        return ((double) currentNano / (double) loopNano); //% of raw time elapsed
    }

    //testing sine in ease
    //double easing(double perc) {
    //  return -cos(perc * (M_PI * .5)) + 1.0;
    // }

    double Behavior::calculateOffsetPercentage(double perc, double prevPerc, int dir) {

        if (isEasing) {
            // printf("perc = %f, prevPrec = %f, offPerc = %f\n", perc, prevPerc, (perc - prevPerc));
            double offPerc = m_easing->easeFunc(perc) - m_easing->easeFunc(prevPerc);
            return offPerc * dir;
        } else {
            return (perc - prevPerc) * dir;
        }
    }


    void Behavior::loopBehavior() {
        nextTime += loopLength;

        prevPercentage = overshoot;

        // if (easing == null)
        {
            offsetPercentage += (-1.0 + (overshoot));
        }

        //   else
        //   {
        //   this.offsetPercentage += (-1f + (easing.getPercentage(overshoot)));
        //   }

    }

    void Behavior::reverseBehavior() {
        dir *= -1;
        nextTime += loopLength;

        //handle overshoot
        prevPercentage = overshoot;

        //if (easing == null)
        {
            offsetPercentage += overshoot * dir;
        }
        //  else
        //  {
        //    this.offsetPercentage += easing.getPercentage(overshoot) * dir;
        //  }
    }


    Behavior &Behavior::tick(long tick) {

        m_offsets = vec3(0.0);


        // timeToLoop = false;

        //double tick = _tick;

        /*
           if (isInterrupted == true && interruptNano <= getTick())
           {
        //System.err.println("INTERRUPTED!");
        this.isDone = (true);
        return;
        }
        */

        if (tick < nextTime) {
            isActive = false;
            //printf("too soon!!!!!!!!!!!!!!!!!!! %lu < %lu \n", tick, nextTime);

            return *this;
        }

        //printf("tick =  %lu \n", tick);



        isActive = true;
        percentage = getRawPercentage(tick - nextTime, loopLength);

        if (percentage < 0.0) {
            //printf("somehow percentage < 0!  %f\n", percentage);
        }

        if (percentage >= 1.0) {
            //printf("we went too far!!!!!!!!!!!!!!!!!!! %f\n", percentage);
            //printf("we went too far , prevPercentage = %f\n", prevPercentage);

            overshoot = percentage - 1.0;
            percentage = 1.0;
            offsetPercentage = calculateOffsetPercentage(percentage, prevPercentage, dir);

            //printf("so only go this far... offset percentage............... %f\n", offsetPercentage);

            prevPercentage = percentage;

            //   timeToLoop = true;

            if (numRepeats > -1) //-1 is our code for repeating forever
            {
                repeat++;
                if (repeat >= numRepeats) {

                    if (isLooping == true) {

                        offsetPercentage = calculateOffsetPercentage(0.0, 1.0 - offsetPercentage, dir);
                        //printf("last looping... offset percentage... %f\n", offsetPercentage);
                        m_offsets += ranges * (float) offsetPercentage;
                        m_totals += m_offsets;
                    } else {
                        m_offsets += ranges * (float) offsetPercentage;
                        m_totals += m_offsets;
                    }

                    isDone = true;

                    return *this;
                }
            }


            if (isReversing == true) {
                reverseBehavior();
            }
            else if (isLooping == true) {
                loopBehavior();
            }


            //    else
            //    {
            //      continueBehavior();
            //    }

        } else {
            offsetPercentage = calculateOffsetPercentage(percentage, prevPercentage, dir);

            // System.out.println("\n***percentage = " + percentage);
            //      System.out.println("prev percentage = " + prevPercentage);
            //      System.out.println("eased percentage = " + easing.getPercentage(percentage));
            //      System.out.println("eased ppercentage = " + easing.getPercentage(prevPercentage));
            //      System.out.println("offsetPercentage = " + offsetPercentage + "***\n");
            prevPercentage = percentage;
        }

        m_offsets += ranges * (float) offsetPercentage;
        m_totals += m_offsets;

        return *this;
    }

};

