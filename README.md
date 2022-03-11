# Spectra
Spectra of Au+Au collisions, RCP
Мой код состоит из двух этапов:
(I) - код построения спектров, обрабатываемый на ферме
(II) - код построения RCP, производящийся локально

(I) https://github.com/ManukhovStepan/Spectra/blob/main/Spectre/Analysis27GeV.C
Схема работы
1) Открывается входной файл, создается выходной файл
2) Инициализируются вспомогательные переменные и гистограммы
3) Определяются гистрограммы, среди которых:
hNormCent9 - гистограмма, содержащая в себе количество событий в каждом классе центральности
hCharge - гистограмма, содержащая в себе количество положительных и отрицательных трэков
hPosSpectra[9] - 9 гистограмм, включающих в себя положительные спектры для 9 классов центральности (0-5, 5-10, 10-20, 20-30, 30-40, 40-50, 50-60, 60-70, 70-80)
hNegSpectra[9] - тоже, только отрицательные
hPos60_80, hNeg60_80 - объединенные спектры для класса центральности 60-80
4) Задаются параметры отрисовки
5) Задаются только две ветки (Event и Track) во входном файле
6) ЗАПУСКАЕТСЯ СЧИТЫВАНИЕ ПО СОБЫТИЯМ
7) Производится отбор по MB триггерам
8) Определяется класс центральности с помощью класса StRefMultCorr, определяется вес по центральности
СentWeight = refmultCorrUtil->getWeight();
Производится отбор по БэдРанам и сентральности < 0:
if(isBadRun) 	continue;
if(cent9<0)            continue;
9) Вводятся ограничения на события (event cuts)
if(abs(event->primaryVertex().Z())>30.) continue;
if(rV> 1.)	   			                       continue;
10) Заполняется гистограмма hNormCent9  ->Fill(cent9);
11) ЗАПУСКАЕТСЯ СЧИТЫВАНИЕ ПО СОБЫТИЯМ
12) Проверяется первичность трека
13) Вводястя ограничения на треки (track cuts)
if(picoTrack->gDCA(event->primaryVertex()).Mag()>1.)		continue;
if(abs(picoTrack->pMom().Eta())>0.5)				continue;
if(picoTrack->nHitsFit()<16.) 					continue;
if(nHitsOverPoss<0.52) 						continue;
if(picoTrack->nHitsDedx()<11.)  				continue; // (?)
if(picoTrack->pMom().Pt()<0.2) 					continue;
if((picoTrack->gMom().Pt() / picoTrack->pMom().Pt())<0.7)	continue;
if((picoTrack->gMom().Pt() / picoTrack->pMom().Pt())>1.4285)	continue;
14) Происходит заполнение спектров
В качестве веса берется 
CentWeight* 1./(2.*pi*pT*dPt*dEta) = вес
Затем заполнение:
hPosSpectra[cent9]-> Fill(pT, вес);
Если событие перефирическое, то
if(cent9<2) { hPos60_80->Fill(pT, вес);}
Здесь же заполняется гистограмма hCharge
15) КОНЕЦ СЧИТЫВАНИЯ ПО ТРЕКАМ
16) КОНЕЦ СЧИТЫВАНИЯ ПО СОБЫТИЯМ
17) Запись гистограмм в файл, его сохранение и закрытие

После этой обработки на выходе получаются ненормированные спектры, внутри которых скаладывались треки всех событий вместе (конечно, в данном классе центральности, с данным поперечным импульсом и данной псевдобыстротой)

Так как после обработки на ферме создается множество файлов, все они объядиняются с помощью комманды
hadd 27GeV_total.root *.root 
которая, как известно, просто суммирует все внутренние гистограммы. Этот файл 27GeV_total.root выгружается с фермы и запускается следующий код

(II) https://github.com/ManukhovStepan/Spectra/blob/main/Plotting/RCP_divide_method.cpp
1) Открывается файл и достаются гистограммы hPos60_80, hNeg60_80, hPos0_5, hNeg0_5, hNormCent9
Из последней достается количество событий в классах центральности 0-5 и 60-80:
Double_t NEvent0_5 = hCent9->GetBinContent(10);
Double_t NEvent60_80 = hCent9->GetBinContent(2) + hCent9->GetBinContent(3);
2) Производится нормировка гистограмм Pos60_80, hNeg60_80, hPos0_5, hNeg0_5 на эти числа.
ВАЖНО - именно в этот момент кода спектры становятся правильными с точки зрения нормировки. То есть каждый бин этой гистограммы говорит, сколько заряженных частиц рождается в среднем
- на одно событие
- в данном классе центральности
- в интервале поперечного импульса pT до pT+dpT
- в интервале псевдобыстроты dEta
3) Создаются гистограммы RCP с помощью метода divide:
TH1D *hPosRCP;
hPosRCP = (TH1D*)hPos0_5->Clone("PosRCP");
hPosRCP->Divide(hPos60_80);
(аналогично для отрицательных)
4) Производится нормировка на <Ncoll>
5) Отрисовка. Код поддерживает две способа работы
- Первый способ (mode ==1) отрисовывает на одном канвасе  две гистограммы
- Второй способ (mode == 2) отрисовывает усредененный RCP по формуле:
RCPaverage = (N1*RCPpos+N2*RCPneg)/(N1+N2)
где  N1 - количество полжительных треков (всего)
N2 - количество отрицательных треков (всего)
