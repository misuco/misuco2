#ifndef INITXML_H
#define INITXML_H

#include <QByteArray>

const char * data = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE misuco><misuco version=\"2.0\"><scale rootNote=\"0\" baseoct=\"4\" topoct=\"5\" b0=\"0\" b1=\"0\" b2=\"0\" b3=\"0\" b4=\"1\" b5=\"0\" b6=\"0\" b7=\"1\" b8=\"0\" b9=\"0\" b10=\"0\"/><scale rootNote=\"0\" baseoct=\"4\" topoct=\"5\" b0=\"0\" b1=\"0\" b2=\"0\" b3=\"0\" b4=\"1\" b5=\"0\" b6=\"0\" b7=\"0\" b8=\"1\" b9=\"0\" b10=\"0\"/><scale rootNote=\"0\" baseoct=\"4\" topoct=\"5\" b0=\"0\" b1=\"1\" b2=\"1\" b3=\"0\" b4=\"1\" b5=\"0\" b6=\"1\" b7=\"0\" b8=\"1\" b9=\"1\" b10=\"0\"/><scale rootNote=\"2\" baseoct=\"4\" topoct=\"5\" b0=\"0\" b1=\"1\" b2=\"1\" b3=\"0\" b4=\"1\" b5=\"0\" b6=\"1\" b7=\"0\" b8=\"1\" b9=\"1\" b10=\"0\"/><scale rootNote=\"0\" baseoct=\"4\" topoct=\"5\" b0=\"1\" b1=\"0\" b2=\"1\" b3=\"0\" b4=\"1\" b5=\"0\" b6=\"1\" b7=\"1\" b8=\"0\" b9=\"1\" b10=\"0\"/><scale rootNote=\"2\" baseoct=\"4\" topoct=\"5\" b0=\"1\" b1=\"0\" b2=\"1\" b3=\"0\" b4=\"0\" b5=\"1\" b6=\"0\" b7=\"1\" b8=\"0\" b9=\"0\" b10=\"1\"/><sound wave=\"1\" attack=\"8\" decay=\"91\" sustain=\"939.000000\" release=\"761\" cutoff=\"43.000000\" resonance=\"30.000000\" mod_cutoff=\"50.000000\" mod_resonance=\"0.000000\" volume=\"800.000000\"/><sound wave=\"2\" attack=\"20\" decay=\"467\" sustain=\"345.000000\" release=\"693\" cutoff=\"112.000000\" resonance=\"0.000000\" mod_cutoff=\"50.000000\" mod_resonance=\"0.000000\" volume=\"917.000000\"/><sound wave=\"0\" attack=\"20\" decay=\"467\" sustain=\"345.000000\" release=\"693\" cutoff=\"87.000000\" resonance=\"327.000000\" mod_cutoff=\"845.000000\" mod_resonance=\"0.000000\" volume=\"917.000000\"/><sound wave=\"2\" attack=\"20\" decay=\"467\" sustain=\"345.000000\" release=\"693\" cutoff=\"289.000000\" resonance=\"446.000000\" mod_cutoff=\"518.000000\" mod_resonance=\"0.000000\" volume=\"917.000000\"/><sound wave=\"0\" attack=\"17\" decay=\"106\" sustain=\"187.000000\" release=\"929\" cutoff=\"731.000000\" resonance=\"76.000000\" mod_cutoff=\"50.000000\" mod_resonance=\"0.000000\" volume=\"800.000000\"/><sound wave=\"1\" attack=\"92\" decay=\"179\" sustain=\"629.000000\" release=\"224\" cutoff=\"725.000000\" resonance=\"231.000000\" mod_cutoff=\"646.000000\" mod_resonance=\"0.000000\" volume=\"800.000000\"/><microtune t0=\"-1\" t1=\"0\" t2=\"0\" t3=\"0\" t4=\"0\" t5=\"0\" t6=\"0\" t7=\"0\" t8=\"0\" t9=\"0\" t10=\"0\" t11=\"0\"/><microtune t0=\"-10\" t1=\"0\" t2=\"0\" t3=\"0\" t4=\"0\" t5=\"0\" t6=\"0\" t7=\"0\" t8=\"0\" t9=\"0\" t10=\"0\" t11=\"0\"/><microtune t0=\"-50\" t1=\"0\" t2=\"0\" t3=\"0\" t4=\"0\" t5=\"0\" t6=\"0\" t7=\"0\" t8=\"0\" t9=\"0\" t10=\"0\" t11=\"0\"/><microtune t0=\"20\" t1=\"0\" t2=\"0\" t3=\"0\" t4=\"0\" t5=\"0\" t6=\"0\" t7=\"0\" t8=\"0\" t9=\"0\" t10=\"0\" t11=\"0\"/><microtune t0=\"40\" t1=\"0\" t2=\"0\" t3=\"0\" t4=\"0\" t5=\"0\" t6=\"0\" t7=\"0\" t8=\"0\" t9=\"0\" t10=\"0\" t11=\"0\"/><microtune t0=\"34\" t1=\"0\" t2=\"-20\" t3=\"0\" t4=\"41\" t5=\"0\" t6=\"-25\" t7=\"0\" t8=\"33\" t9=\"0\" t10=\"-15\" t11=\"0\"/><setup pitchTopRange=\"2\" pitchBottomRange=\"0\" pitchHorizontal=\"0\" channel=\"0\" sendCC1=\"1\" bwmode=\"1\" mobileSynth=\"1\" pureData=\"1\" reaktor=\"0\" superCollider=\"0\" holdMode=\"0\" noteSymbols=\"0\" showFreqs=\"0\" showH0=\"0\" showH1=\"1\" showH2=\"0\" showM0=\"1\" showM1=\"0\" showM2=\"0\" showM3=\"0\"/></misuco>";

QByteArray initXml(data);

#endif // INITXML_H
