//
//  Spectrum.cpp
//
//  Created by 渡部 心 on 2015/09/21.
//  Copyright (c) 2015年 渡部 心. All rights reserved.
//

#include "Spectrum.h"
#include "../BasicTypes/CompensatedSum.h"

namespace SLR {
    const float xbar_2deg[] = {
        0.0001299, 0.000145847, 0.000163802, 0.000184004, 0.00020669, 0.0002321, 0.000260728, 0.000293075,
        0.000329388, 0.000369914, 0.0004149, 0.000464159, 0.000518986, 0.000581854, 0.000655235, 0.0007416,
        0.00084503, 0.000964527, 0.001094949, 0.001231154, 0.001368, 0.00150205, 0.001642328, 0.001802382,
        0.001995757, 0.002236, 0.002535385, 0.002892603, 0.003300829, 0.003753236, 0.004243, 0.004762389,
        0.005330048, 0.005978712, 0.006741117, 0.00765, 0.008751373, 0.01002888, 0.0114217, 0.01286901,
        0.01431, 0.01570443, 0.01714744, 0.01878122, 0.02074801, 0.02319, 0.02620736, 0.02978248,
        0.03388092, 0.03846824, 0.04351, 0.0489956, 0.0550226, 0.0617188, 0.069212, 0.07763,
        0.08695811, 0.09717672, 0.1084063, 0.1207672, 0.13438, 0.1493582, 0.1653957, 0.1819831,
        0.198611, 0.21477, 0.2301868, 0.2448797, 0.2587773, 0.2718079, 0.2839, 0.2949438,
        0.3048965, 0.3137873, 0.3216454, 0.3285, 0.3343513, 0.3392101, 0.3431213, 0.3461296,
        0.34828, 0.3495999, 0.3501474, 0.350013, 0.349287, 0.34806, 0.3463733, 0.3442624,
        0.3418088, 0.3390941, 0.3362, 0.3331977, 0.3300411, 0.3266357, 0.3228868, 0.3187,
        0.3140251, 0.308884, 0.3032904, 0.2972579, 0.2908, 0.2839701, 0.2767214, 0.2689178,
        0.2604227, 0.2511, 0.2408475, 0.2298512, 0.2184072, 0.2068115, 0.19536, 0.1842136,
        0.1733273, 0.1626881, 0.1522833, 0.1421, 0.1321786, 0.1225696, 0.1132752, 0.1042979,
        0.09564, 0.08729955, 0.07930804, 0.07171776, 0.06458099, 0.05795001, 0.05186211, 0.04628152,
        0.04115088, 0.03641283, 0.03201, 0.0279172, 0.0241444, 0.020687, 0.0175404, 0.0147,
        0.01216179, 0.00991996, 0.00796724, 0.006296346, 0.0049, 0.003777173, 0.00294532, 0.00242488,
        0.002236293, 0.0024, 0.00292552, 0.00383656, 0.00517484, 0.00698208, 0.0093, 0.01214949,
        0.01553588, 0.01947752, 0.02399277, 0.0291, 0.03481485, 0.04112016, 0.04798504, 0.05537861,
        0.06327, 0.07163501, 0.08046224, 0.08973996, 0.09945645, 0.1096, 0.1201674, 0.1311145,
        0.1423679, 0.1538542, 0.1655, 0.1772571, 0.18914, 0.2011694, 0.2133658, 0.2257499,
        0.2383209, 0.2510668, 0.2639922, 0.2771017, 0.2904, 0.3038912, 0.3175726, 0.3314384,
        0.3454828, 0.3597, 0.3740839, 0.3886396, 0.4033784, 0.4183115, 0.4334499, 0.4487953,
        0.464336, 0.480064, 0.4959713, 0.5120501, 0.5282959, 0.5446916, 0.5612094, 0.5778215,
        0.5945, 0.6112209, 0.6279758, 0.6447602, 0.6615697, 0.6784, 0.6952392, 0.7120586,
        0.7288284, 0.7455188, 0.7621, 0.7785432, 0.7948256, 0.8109264, 0.8268248, 0.8425,
        0.8579325, 0.8730816, 0.8878944, 0.9023181, 0.9163, 0.9297995, 0.9427984, 0.9552776,
        0.9672179, 0.9786, 0.9893856, 0.9995488, 1.0090892, 1.0180064, 1.0263, 1.0339827,
        1.040986, 1.047188, 1.0524667, 1.0567, 1.0597944, 1.0617992, 1.0628068, 1.0629096,
        1.0622, 1.0607352, 1.0584436, 1.0552244, 1.0509768, 1.0456, 1.0390369, 1.0313608,
        1.0226662, 1.0130477, 1.0026, 0.9913675, 0.9793314, 0.9664916, 0.9528479, 0.9384,
        0.923194, 0.907244, 0.890502, 0.87292, 0.8544499, 0.835084, 0.814946, 0.794186,
        0.772954, 0.7514, 0.7295836, 0.7075888, 0.6856022, 0.6638104, 0.6424, 0.6215149,
        0.6011138, 0.5811052, 0.5613977, 0.5419, 0.5225995, 0.5035464, 0.4847436, 0.4661939,
        0.4479, 0.4298613, 0.412098, 0.394644, 0.3775333, 0.3608, 0.3444563, 0.3285168,
        0.3130192, 0.2980011, 0.2835, 0.2695448, 0.2561184, 0.2431896, 0.2307272, 0.2187,
        0.2070971, 0.1959232, 0.1851708, 0.1748323, 0.1649, 0.1553667, 0.14623, 0.13749,
        0.1291467, 0.1212, 0.1136397, 0.106465, 0.09969044, 0.09333061, 0.0874, 0.08190096,
        0.07680428, 0.07207712, 0.06768664, 0.0636, 0.05980685, 0.05628216, 0.05297104, 0.04981861,
        0.04677, 0.04378405, 0.04087536, 0.03807264, 0.03540461, 0.0329, 0.03056419, 0.02838056,
        0.02634484, 0.02445275, 0.0227, 0.02108429, 0.01959988, 0.01823732, 0.01698717, 0.01584,
        0.01479064, 0.01383132, 0.01294868, 0.0121292, 0.01135916, 0.01062935, 0.009938846, 0.009288422,
        0.008678854, 0.008110916, 0.007582388, 0.007088746, 0.006627313, 0.006195408, 0.005790346, 0.005409826,
        0.005052583, 0.004717512, 0.004403507, 0.004109457, 0.003833913, 0.003575748, 0.003334342, 0.003109075,
        0.002899327, 0.002704348, 0.00252302, 0.002354168, 0.002196616, 0.00204919, 0.00191096, 0.001781438,
        0.00166011, 0.001546459, 0.001439971, 0.001340042, 0.001246275, 0.001158471, 0.00107643, 0.000999949,
        0.000928736, 0.000862433, 0.00080075, 0.000743396, 0.000690079, 0.000640516, 0.000594502, 0.000551865,
        0.000512429, 0.000476021, 0.000442454, 0.000411512, 0.000382981, 0.000356649, 0.000332301, 0.000309759,
        0.000288887, 0.000269539, 0.000251568, 0.000234826, 0.000219171, 0.000204526, 0.000190841, 0.000178065,
        0.000166151, 0.000155024, 0.000144622, 0.00013491, 0.000125852, 0.000117413, 0.000109552, 0.000102225,
        9.53945E-05, 8.90239E-05, 8.30753E-05, 7.75127E-05, 7.2313E-05, 6.74578E-05, 6.29284E-05, 5.87065E-05,
        5.47703E-05, 5.10992E-05, 4.76765E-05, 4.44857E-05, 4.15099E-05, 3.87332E-05, 3.6142E-05, 3.37235E-05,
        3.14649E-05, 2.93533E-05, 2.73757E-05, 2.55243E-05, 2.37938E-05, 2.21787E-05, 2.06738E-05, 1.92723E-05,
        1.79664E-05, 1.67499E-05, 1.56165E-05, 1.45598E-05, 1.35739E-05, 1.26544E-05, 1.17972E-05, 1.09984E-05,
        1.0254E-05, 9.55965E-06, 8.91204E-06, 8.30836E-06, 7.74577E-06, 7.22146E-06, 6.73248E-06, 6.27642E-06,
        5.8513E-06, 5.45512E-06, 5.08587E-06, 4.74147E-06, 4.42024E-06, 4.12078E-06, 3.84172E-06, 3.58165E-06,
        3.33913E-06, 3.11295E-06, 2.90212E-06, 2.70565E-06, 2.52253E-06, 2.35173E-06, 2.19242E-06, 2.0439E-06,
        1.9055E-06, 1.77651E-06, 1.65622E-06, 1.54402E-06, 1.43944E-06, 1.34198E-06, 1.25114E-06
    };
    
    const float ybar_2deg[] = {
        0.000003917, 4.39358E-06, 4.9296E-06, 5.53214E-06, 6.20825E-06, 0.000006965, 7.81322E-06, 8.76734E-06,
        9.83984E-06, 1.10432E-05, 0.00001239, 1.38864E-05, 1.55573E-05, 1.7443E-05, 1.95838E-05, 0.00002202,
        2.48397E-05, 2.80413E-05, 3.1531E-05, 3.52152E-05, 0.000039, 4.28264E-05, 4.69146E-05, 5.15896E-05,
        5.71764E-05, 0.000064, 7.23442E-05, 8.22122E-05, 9.35082E-05, 0.000106136, 0.00012, 0.000134984,
        0.000151492, 0.000170208, 0.000191816, 0.000217, 0.000246907, 0.00028124, 0.00031852, 0.000357267,
        0.000396, 0.000433715, 0.000473024, 0.000517876, 0.000572219, 0.00064, 0.00072456, 0.0008255,
        0.00094116, 0.00106988, 0.00121, 0.001362091, 0.001530752, 0.001720368, 0.001935323, 0.00218,
        0.0024548, 0.002764, 0.0031178, 0.0035264, 0.004, 0.00454624, 0.00515932, 0.00582928,
        0.00654616, 0.0073, 0.008086507, 0.00890872, 0.00976768, 0.01066443, 0.0116, 0.01257317,
        0.01358272, 0.01462968, 0.01571509, 0.01684, 0.01800736, 0.01921448, 0.02045392, 0.02171824,
        0.023, 0.02429461, 0.02561024, 0.02695857, 0.02835125, 0.0298, 0.03131083, 0.03288368,
        0.03452112, 0.03622571, 0.038, 0.03984667, 0.041768, 0.043766, 0.04584267, 0.048,
        0.05024368, 0.05257304, 0.05498056, 0.05745872, 0.06, 0.06260197, 0.06527752, 0.06804208,
        0.07091109, 0.0739, 0.077016, 0.0802664, 0.0836668, 0.0872328, 0.09098, 0.09491755,
        0.09904584, 0.1033674, 0.1078846, 0.1126, 0.117532, 0.1226744, 0.1279928, 0.1334528,
        0.13902, 0.1446764, 0.1504693, 0.1564619, 0.1627177, 0.1693, 0.1762431, 0.1835581,
        0.1912735, 0.199418, 0.20802, 0.2171199, 0.2267345, 0.2368571, 0.2474812, 0.2586,
        0.2701849, 0.2822939, 0.2950505, 0.308578, 0.323, 0.3384021, 0.3546858, 0.3716986,
        0.3892875, 0.4073, 0.4256299, 0.4443096, 0.4633944, 0.4829395, 0.503, 0.5235693,
        0.544512, 0.56569, 0.5869653, 0.6082, 0.6293456, 0.6503068, 0.6708752, 0.6908424,
        0.71, 0.7281852, 0.7454636, 0.7619694, 0.7778368, 0.7932, 0.8081104, 0.8224962,
        0.8363068, 0.8494916, 0.862, 0.8738108, 0.8849624, 0.8954936, 0.9054432, 0.9148501,
        0.9237348, 0.9320924, 0.9399226, 0.9472252, 0.954, 0.9602561, 0.9660074, 0.9712606,
        0.9760225, 0.9803, 0.9840924, 0.9874182, 0.9903128, 0.9928116, 0.9949501, 0.9967108,
        0.9980983, 0.999112, 0.9997482, 1, 0.9998567, 0.9993046, 0.9983255, 0.9968987,
        0.995, 0.9926005, 0.9897426, 0.9864444, 0.9827241, 0.9786, 0.9740837, 0.9691712,
        0.9638568, 0.9581349, 0.952, 0.9454504, 0.9384992, 0.9311628, 0.9234576, 0.9154,
        0.9070064, 0.8982772, 0.8892048, 0.8797816, 0.87, 0.8598613, 0.849392, 0.838622,
        0.8275813, 0.8163, 0.8047947, 0.793082, 0.781192, 0.7691547, 0.757, 0.7447541,
        0.7324224, 0.7200036, 0.7074965, 0.6949, 0.6822192, 0.6694716, 0.6566744, 0.6438448,
        0.631, 0.6181555, 0.6053144, 0.5924756, 0.5796379, 0.5668, 0.5539611, 0.5411372,
        0.5283528, 0.5156323, 0.503, 0.4904688, 0.4780304, 0.4656776, 0.4534032, 0.4412,
        0.42908, 0.417036, 0.405032, 0.393032, 0.381, 0.3689184, 0.3568272, 0.3447768,
        0.3328176, 0.321, 0.3093381, 0.2978504, 0.2865936, 0.2756245, 0.265, 0.2547632,
        0.2448896, 0.2353344, 0.2260528, 0.217, 0.2081616, 0.1995488, 0.1911552, 0.1829744,
        0.175, 0.1672235, 0.1596464, 0.1522776, 0.1451259, 0.1382, 0.1315003, 0.1250248,
        0.1187792, 0.1127691, 0.107, 0.1014762, 0.09618864, 0.09112296, 0.08626485, 0.0816,
        0.07712064, 0.07282552, 0.06871008, 0.06476976, 0.061, 0.05739621, 0.05395504, 0.05067376,
        0.04754965, 0.04458, 0.04175872, 0.03908496, 0.03656384, 0.03420048, 0.032, 0.02996261,
        0.02807664, 0.02632936, 0.02470805, 0.0232, 0.02180077, 0.02050112, 0.01928108, 0.01812069,
        0.017, 0.01590379, 0.01483718, 0.01381068, 0.01283478, 0.01192, 0.01106831, 0.01027339,
        0.009533311, 0.008846157, 0.00821, 0.007623781, 0.007085424, 0.006591476, 0.006138485, 0.005723,
        0.005343059, 0.004995796, 0.004676404, 0.004380075, 0.004102, 0.003838453, 0.003589099, 0.003354219,
        0.003134093, 0.002929, 0.002738139, 0.002559876, 0.002393244, 0.002237275, 0.002091, 0.001953587,
        0.00182458, 0.00170358, 0.001590187, 0.001484, 0.001384496, 0.001291268, 0.001204092, 0.001122744,
        0.001047, 0.00097659, 0.000911109, 0.000850133, 0.000793238, 0.00074, 0.000690083, 0.00064331,
        0.000599496, 0.000558455, 0.00052, 0.000483914, 0.000450053, 0.000418345, 0.000388718, 0.0003611,
        0.000335384, 0.00031144, 0.000289166, 0.000268454, 0.0002492, 0.000231302, 0.000214686, 0.000199288,
        0.000185048, 0.0001719, 0.000159778, 0.000148604, 0.000138302, 0.000128793, 0.00012, 0.00011186,
        0.000104322, 9.73356E-05, 9.08459E-05, 0.0000848, 7.91467E-05, 0.000073858, 0.000068916, 6.43027E-05,
        0.00006, 5.59819E-05, 5.22256E-05, 4.87184E-05, 4.54475E-05, 0.0000424, 3.9561E-05, 3.69151E-05,
        3.44487E-05, 3.21482E-05, 0.00003, 2.79913E-05, 2.61136E-05, 2.43602E-05, 2.27246E-05, 0.0000212,
        1.97786E-05, 1.84529E-05, 1.72169E-05, 1.60646E-05, 0.00001499, 1.39873E-05, 1.30516E-05, 1.21782E-05,
        1.13625E-05, 0.0000106, 9.88588E-06, 9.2173E-06, 8.59236E-06, 8.00913E-06, 7.4657E-06, 6.95957E-06,
        6.488E-06, 6.0487E-06, 5.6394E-06, 5.2578E-06, 4.90177E-06, 4.56972E-06, 4.26019E-06, 3.97174E-06,
        3.7029E-06, 3.45216E-06, 3.2183E-06, 3.0003E-06, 2.79714E-06, 2.6078E-06, 2.43122E-06, 2.26653E-06,
        2.11301E-06, 1.96994E-06, 1.8366E-06, 1.71223E-06, 1.59623E-06, 1.48809E-06, 1.38731E-06, 1.2934E-06,
        1.20582E-06, 1.12414E-06, 1.04801E-06, 9.77058E-07, 9.1093E-07, 8.49251E-07, 7.91721E-07, 7.3809E-07,
        6.8811E-07, 6.4153E-07, 5.9809E-07, 5.57575E-07, 5.19808E-07, 4.84612E-07, 4.5181E-07
    };
    
    const float zbar_2deg[] = {
        0.0006061, 0.000680879, 0.000765146, 0.000860012, 0.000966593, 0.001086, 0.001220586, 0.001372729,
        0.001543579, 0.001734286, 0.001946, 0.002177777, 0.002435809, 0.002731953, 0.003078064, 0.003486,
        0.003975227, 0.00454088, 0.00515832, 0.005802907, 0.006450001, 0.007083216, 0.007745488, 0.008501152,
        0.009414544, 0.01054999, 0.0119658, 0.01365587, 0.01558805, 0.01773015, 0.02005001, 0.02251136,
        0.02520288, 0.02827972, 0.03189704, 0.03621, 0.04143771, 0.04750372, 0.05411988, 0.06099803,
        0.06785001, 0.07448632, 0.08136156, 0.08915364, 0.09854048, 0.1102, 0.1246133, 0.1417017,
        0.1613035, 0.1832568, 0.2074, 0.2336921, 0.2626114, 0.2947746, 0.3307985, 0.3713,
        0.4162091, 0.4654642, 0.5196948, 0.5795303, 0.6456, 0.7184838, 0.7967133, 0.8778459,
        0.959439, 1.0390501, 1.1153673, 1.1884971, 1.2581233, 1.3239296, 1.3856, 1.4426352,
        1.4948035, 1.5421903, 1.5848807, 1.62296, 1.6564048, 1.6852959, 1.7098745, 1.7303821,
        1.74706, 1.7600446, 1.7696233, 1.7762637, 1.7804334, 1.7826, 1.7829682, 1.7816998,
        1.7791982, 1.7758671, 1.77211, 1.7682589, 1.764039, 1.7589438, 1.7524663, 1.7441,
        1.7335595, 1.7208581, 1.7059369, 1.6887372, 1.6692, 1.6475287, 1.6234127, 1.5960223,
        1.564528, 1.5281, 1.4861114, 1.4395215, 1.3898799, 1.3387362, 1.28764, 1.2374223,
        1.1878243, 1.1387611, 1.090148, 1.0419, 0.9941976, 0.9473473, 0.9014531, 0.8566193,
        0.8129501, 0.7705173, 0.7294448, 0.6899136, 0.6521049, 0.6162, 0.5823286, 0.5504162,
        0.5203376, 0.4919673, 0.46518, 0.4399246, 0.4161836, 0.3938822, 0.3729459, 0.3533,
        0.3348578, 0.3175521, 0.3013375, 0.2861686, 0.272, 0.2588171, 0.2464838, 0.2347718,
        0.2234533, 0.2123, 0.2011692, 0.1901196, 0.1792254, 0.1685608, 0.1582, 0.1481383,
        0.1383758, 0.1289942, 0.1200751, 0.1117, 0.1039048, 0.09666748, 0.08998272, 0.08384531,
        0.07824999, 0.07320899, 0.06867816, 0.06456784, 0.06078835, 0.05725001, 0.05390435, 0.05074664,
        0.04775276, 0.04489859, 0.04216, 0.03950728, 0.03693564, 0.03445836, 0.03208872, 0.02984,
        0.02771181, 0.02569444, 0.02378716, 0.02198925, 0.0203, 0.01871805, 0.01724036, 0.01586364,
        0.01458461, 0.0134, 0.01230723, 0.01130188, 0.01037792, 0.009529306, 0.008749999, 0.0080352,
        0.0073816, 0.0067854, 0.0062428, 0.005749999, 0.0053036, 0.0048998, 0.0045342, 0.0042024,
        0.0039, 0.0036232, 0.0033706, 0.0031414, 0.0029348, 0.002749999, 0.0025852, 0.0024386,
        0.0023094, 0.0021968, 0.0021, 0.002017733, 0.0019482, 0.0018898, 0.001840933, 0.0018,
        0.001766267, 0.0017378, 0.0017112, 0.001683067, 0.001650001, 0.001610133, 0.0015644, 0.0015136,
        0.001458533, 0.0014, 0.001336667, 0.00127, 0.001205, 0.001146667, 0.0011, 0.0010688,
        0.0010494, 0.0010356, 0.0010212, 0.001, 0.00096864, 0.00092992, 0.00088688, 0.00084256,
        0.0008, 0.00076096, 0.00072368, 0.00068592, 0.00064544, 0.0006, 0.000547867, 0.0004916,
        0.0004354, 0.000383467, 0.00034, 0.000307253, 0.00028316, 0.00026544, 0.000251813, 0.00024,
        0.000229547, 0.00022064, 0.00021196, 0.000202187, 0.00019, 0.000174213, 0.00015564, 0.00013596,
        0.000116853, 0.0001, 8.61333E-05, 0.0000746, 0.000065, 5.69333E-05, 5E-05, 0.00004416,
        0.00003948, 0.00003572, 0.00003264, 0.00003, 2.76533E-05, 0.00002556, 0.00002364, 2.18133E-05,
        0.00002, 1.81333E-05, 0.0000162, 0.0000142, 1.21333E-05, 0.00001, 7.73333E-06, 0.0000054,
        0.0000032, 1.33333E-06, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    
    
    static SpectrumFloat integralCMF;
    
    void initSpectrum() {
        DiscretizedSpectrum::init();
        
        CompensatedSum<SpectrumFloat> cum(0);
        for (int i = 1; i < NumCMFSamples; ++i)
            cum += (ybar_2deg[i - 1] + ybar_2deg[i]) * 1 * 0.5;
        integralCMF = cum;
    }
    
#ifdef Use_Spectral_Representation
    InputSpectrumRef createInputSpectrum(SpectrumType spType, ColorSpace space, SpectrumFloat e0, SpectrumFloat e1, SpectrumFloat e2) {
        return createShared<UpsampledContinuousSpectrum>(spType, space, e0, e1, e2);
    }
    InputSpectrumRef createInputSpectrum(SpectrumType spType, SpectrumFloat minLambda, SpectrumFloat maxLambda, const SpectrumFloat* values, uint32_t numSamples) {
        return createShared<RegularContinuousSpectrum>(minLambda, maxLambda, values, numSamples);
    }
    InputSpectrumRef createInputSpectrum(SpectrumType spType, const SpectrumFloat* lambdas, const SpectrumFloat* values, uint32_t numSamples) {
        return createShared<IrregularContinuousSpectrum>(lambdas, values, numSamples);
    }
#else
    static void spectrum_to_XYZ(SpectrumFloat minLambda, SpectrumFloat maxLambda, const SpectrumFloat* values, uint32_t numSamples, SpectrumFloat XYZ[3]) {
        const SpectrumFloat CMFBinWidth = (WavelengthHighBound - WavelengthLowBound) / (NumCMFSamples - 1);
        const SpectrumFloat binWidth = (maxLambda - minLambda) / (numSamples - 1);
        uint32_t curCMFIdx = 0;
        uint32_t baseIdx = 0;
        SpectrumFloat curWL = WavelengthLowBound;
        SpectrumFloat prev_xbarVal = 0, prev_ybarVal = 0, prev_zbarVal = 0;
        SpectrumFloat prevValue = 0;
        SpectrumFloat halfWidth = 0;
        CompensatedSum<SpectrumFloat> X(0), Y(0), Z(0);
        while (true) {
            SpectrumFloat xbarValue, ybarValue, zbarValue;
            if (curWL == WavelengthLowBound + curCMFIdx * CMFBinWidth) {
                xbarValue = xbar_2deg[curCMFIdx];
                ybarValue = ybar_2deg[curCMFIdx];
                zbarValue = zbar_2deg[curCMFIdx];
                ++curCMFIdx;
            }
            else {
                uint32_t idx = std::min(uint32_t((curWL - WavelengthLowBound) / CMFBinWidth), NumCMFSamples - 1);
                SpectrumFloat CMFBaseWL = WavelengthLowBound + idx * CMFBinWidth;
                SpectrumFloat t = (curWL - CMFBaseWL) / CMFBinWidth;
                xbarValue = (1 - t) * xbar_2deg[idx] + t * xbar_2deg[idx + 1];
                ybarValue = (1 - t) * ybar_2deg[idx] + t * ybar_2deg[idx + 1];
                zbarValue = (1 - t) * zbar_2deg[idx] + t * zbar_2deg[idx + 1];
            }
            
            SpectrumFloat value;
            if (curWL < minLambda) {
                value = values[0];
            }
            else if (curWL > maxLambda) {
                value = values[numSamples - 1];
            }
            else if (curWL == minLambda + baseIdx * binWidth) {
                value = values[baseIdx];
                ++baseIdx;
            }
            else {
                uint32_t idx = std::min(uint32_t((curWL - minLambda) / binWidth), numSamples - 1);
                SpectrumFloat baseWL = minLambda + idx * binWidth;
                SpectrumFloat t = (curWL - baseWL) / binWidth;
                value = (1 - t) * values[idx] + t * values[idx + 1];
            }
            
            SpectrumFloat avgValue = (prevValue + value) * 0.5f;
            X += avgValue * (prev_xbarVal + xbarValue) * halfWidth;
            Y += avgValue * (prev_ybarVal + ybarValue) * halfWidth;
            Z += avgValue * (prev_zbarVal + zbarValue) * halfWidth;
            
            prev_xbarVal = xbarValue;
            prev_ybarVal = ybarValue;
            prev_zbarVal = zbarValue;
            prevValue = value;
            SpectrumFloat prevWL = curWL;
            curWL = std::min(WavelengthLowBound + curCMFIdx * CMFBinWidth,
                             baseIdx < numSamples ? (minLambda + baseIdx * binWidth) : INFINITY);
            halfWidth = (curWL - prevWL) * 0.5f;
            
            if (curCMFIdx == NumCMFSamples)
                break;
        }
        XYZ[0] = X / integralCMF;
        XYZ[1] = Y / integralCMF;
        XYZ[2] = Z / integralCMF;
    }
    
    static void spectrum_to_XYZ(const SpectrumFloat* lambdas, const SpectrumFloat* values, uint32_t numSamples, SpectrumFloat XYZ[3]) {
        const SpectrumFloat CMFBinWidth = (WavelengthHighBound - WavelengthLowBound) / (NumCMFSamples - 1);
        uint32_t curCMFIdx = 0;
        uint32_t baseIdx = 0;
        SpectrumFloat curWL = WavelengthLowBound;
        SpectrumFloat prev_xbarVal = 0, prev_ybarVal = 0, prev_zbarVal = 0;
        SpectrumFloat prevValue = 0;
        SpectrumFloat halfWidth = 0;
        CompensatedSum<SpectrumFloat> X(0), Y(0), Z(0);
        while (true) {
            SpectrumFloat xbarValue, ybarValue, zbarValue;
            if (curWL == WavelengthLowBound + curCMFIdx * CMFBinWidth) {
                xbarValue = xbar_2deg[curCMFIdx];
                ybarValue = ybar_2deg[curCMFIdx];
                zbarValue = zbar_2deg[curCMFIdx];
                ++curCMFIdx;
            }
            else {
                uint32_t idx = std::min(uint32_t((curWL - WavelengthLowBound) / CMFBinWidth), NumCMFSamples - 1);
                SpectrumFloat CMFBaseWL = WavelengthLowBound + idx * CMFBinWidth;
                SpectrumFloat t = (curWL - CMFBaseWL) / CMFBinWidth;
                xbarValue = (1 - t) * xbar_2deg[idx] + t * xbar_2deg[idx + 1];
                ybarValue = (1 - t) * ybar_2deg[idx] + t * ybar_2deg[idx + 1];
                zbarValue = (1 - t) * zbar_2deg[idx] + t * zbar_2deg[idx + 1];
            }
            
            SpectrumFloat value;
            if (curWL < lambdas[0]) {
                value = values[0];
            }
            else if (curWL > lambdas[1]) {
                value = values[numSamples - 1];
            }
            else if (curWL == lambdas[baseIdx]) {
                value = values[baseIdx];
                ++baseIdx;
            }
            else {
                const SpectrumFloat* lb = std::lower_bound(lambdas + std::max(baseIdx - 1, 0u), lambdas + numSamples, curWL);
                uint32_t idx = std::max(int32_t(std::distance(lambdas, lb)) - 1, 0);
                SpectrumFloat t = (curWL - lambdas[idx]) / (lambdas[idx + 1] - lambdas[idx]);
                value = (1 - t) * values[idx] + t * values[idx + 1];
            }
            
            SpectrumFloat avgValue = (prevValue + value) * 0.5f;
            X += avgValue * (prev_xbarVal + xbarValue) * halfWidth;
            Y += avgValue * (prev_ybarVal + ybarValue) * halfWidth;
            Z += avgValue * (prev_zbarVal + zbarValue) * halfWidth;
            
            prev_xbarVal = xbarValue;
            prev_ybarVal = ybarValue;
            prev_zbarVal = zbarValue;
            prevValue = value;
            SpectrumFloat prevWL = curWL;
            curWL = std::min(WavelengthLowBound + curCMFIdx * CMFBinWidth, baseIdx < numSamples ? lambdas[baseIdx] : INFINITY);
            halfWidth = (curWL - prevWL) * 0.5f;
            
            if (curCMFIdx == NumCMFSamples)
                break;
        }
        XYZ[0] = X / integralCMF;
        XYZ[1] = Y / integralCMF;
        XYZ[2] = Z / integralCMF;
    }
    
    InputSpectrumRef createInputSpectrum(SpectrumType spType, ColorSpace space, SpectrumFloat e0, SpectrumFloat e1, SpectrumFloat e2) {
        SLRAssert(e0 >= 0.0 && e1 >= 0.0 && e2 >= 0.0, "Values should not be minus.");
        switch (space) {
            case ColorSpace::sRGB:
                return createShared<RGBInputSpectrum>(e0, e1, e2);
            case ColorSpace::sRGB_NonLinear: {
                e0 = sRGB_degamma(e0);
                e1 = sRGB_degamma(e1);
                e2 = sRGB_degamma(e2);
                return createShared<RGBInputSpectrum>(e0, e1, e2);
            }
            case ColorSpace::xyY: {
                SpectrumFloat xyY[3] = {e0, e1, e2};
                SpectrumFloat XYZ[3];
                xyY_to_XYZ(xyY, XYZ);
                e0 = XYZ[0];
                e1 = XYZ[1];
                e2 = XYZ[2];
            }
            case ColorSpace::XYZ: {
                SpectrumFloat XYZ[3] = {e0, e1, e2};
                SpectrumFloat RGB[3];
                switch (spType) {
                    case SpectrumType::Reflectance:
                        XYZ_to_sRGB_E(XYZ, RGB);
                        break;
                    case SpectrumType::Illuminant:
                        XYZ_to_sRGB(XYZ, RGB);
                        break;
                    case SpectrumType::IndexOfRefraction:
                        XYZ_to_sRGB_E(XYZ, RGB);
                        break;
                    default:
                        break;
                }
                RGB[0] = RGB[0] < 0.0f ? 0.0f : RGB[0];
                RGB[1] = RGB[1] < 0.0f ? 0.0f : RGB[1];
                RGB[2] = RGB[2] < 0.0f ? 0.0f : RGB[2];
                return createShared<RGBInputSpectrum>(RGB[0], RGB[1], RGB[2]);
            }
            default:
                SLRAssert(false, "Invalid color space.");
                return createShared<RGBInputSpectrum>();
        }
    }
    InputSpectrumRef createInputSpectrum(SpectrumType spType, SpectrumFloat minLambda, SpectrumFloat maxLambda, const SpectrumFloat* values, uint32_t numSamples) {
        SpectrumFloat XYZ[3];
        spectrum_to_XYZ(minLambda, maxLambda, values, numSamples, XYZ);
        SpectrumFloat RGB[3];
        switch (spType) {
            case SpectrumType::Reflectance:
                XYZ_to_sRGB_E(XYZ, RGB);
                break;
            case SpectrumType::Illuminant:
                XYZ_to_sRGB(XYZ, RGB);
                break;
            case SpectrumType::IndexOfRefraction:
                XYZ_to_sRGB_E(XYZ, RGB);
                break;
            default:
                break;
        }
        RGB[0] = RGB[0] < 0.0f ? 0.0f : RGB[0];
        RGB[1] = RGB[1] < 0.0f ? 0.0f : RGB[1];
        RGB[2] = RGB[2] < 0.0f ? 0.0f : RGB[2];
        return createShared<RGBInputSpectrum>(RGB[0], RGB[1], RGB[2]);
    }
    InputSpectrumRef createInputSpectrum(SpectrumType spType, const SpectrumFloat* lambdas, const SpectrumFloat* values, uint32_t numSamples) {
        SpectrumFloat XYZ[3];
        spectrum_to_XYZ(lambdas, values, numSamples, XYZ);
        SpectrumFloat RGB[3];
        switch (spType) {
            case SpectrumType::Reflectance:
                XYZ_to_sRGB_E(XYZ, RGB);
                break;
            case SpectrumType::Illuminant:
                XYZ_to_sRGB(XYZ, RGB);
                break;
            case SpectrumType::IndexOfRefraction:
                XYZ_to_sRGB_E(XYZ, RGB);
                break;
            default:
                break;
        }
        RGB[0] = RGB[0] < 0.0f ? 0.0f : RGB[0];
        RGB[1] = RGB[1] < 0.0f ? 0.0f : RGB[1];
        RGB[2] = RGB[2] < 0.0f ? 0.0f : RGB[2];
        return createShared<RGBInputSpectrum>(RGB[0], RGB[1], RGB[2]);
    }
#endif
}
