// February 21, 2022
#include <string>
#include <vector>
#include <random>
#include <iostream>

#include "fmt/format.h"

template <typename Delimiter>
std::vector<std::string_view> split(std::string_view data, Delimiter&& delim) {
  std::vector<std::string_view> splitted;
  splitted.reserve(data.length() / 2);
  size_t pos = 0;
  while (pos != std::string_view::npos) {
    pos = data.find_first_not_of(delim);
    if (pos == std::string_view::npos) {
      return splitted;
    }
    data = data.substr(pos);
    pos  = data.find_first_of(delim);
    splitted.emplace_back(data.substr(0, pos));
    data = data.substr(pos == std::string_view::npos ? 0 : pos);
  }
  splitted.shrink_to_fit();
  return splitted;
}

auto insults              = split("бык,коронавирус,клоун,кукан,танк,вертолёт,велосипед,батя,братик,процессор,мешок,пакет,коврик,коврижка,чебурек,питон,кал,пёс,инцел,виртуалбокс,долбаёб,долбоёб,далбоёб,планктон,мефедрон,снюс,камаз,туалет,толкан,томат,огурец,банан,ебанан,баклан,ваз-2101,линукс,спрей,поносик,ворон,мусор,понос,помой,карась,хуй,таракан,урод,шпорк,баклажан,овощ,фрукт,сахарок,барсик,пупс,неосарт,линуксоид,виндузятник,маковод,туалет,толкан,толчок,пепел,краб,макинтош,дельфин,трюфель,бсдшник,цыган,чмо,пидор,задрот,кисель,ботан,гандонео,пушок,зефир,негативчик,быдлан,третьеклассник,газ,еблан,уёбок,пидорас,гандон,педик,презик,волос,негр,убунтовод,арчегомосек,шоколад,козёл,бычара,козлище,козён,обама,навальнёнок,говноед,трамп,гей,гомосек,свин,кобель,хохол,сатана", ",");
auto adjectives           = split("ебаный,обоссаный,поднадусёровый,слабонервный,жирный,вонючий,кастрированный,ебучий,невменяемый,блядский,черномазый,оттраханный,обдроченный", ",");
auto geninsult_first      = split("блядо,члено,говно,хуе,желто,черно,много,верто,мало,швайно,глино,гнидо,писько,сопле,криво,пидо,пердо,срало,срано,порно,без", ",");
auto geninsult_second     = split("рылый,жопый,ротый,ебливый,ссущий,срущий,ухий,клювый,зубый,хвостый,бля", ",");
auto geninsult_endings    = split("лёт,ед,блюй,рот,член,мес,пидор,поезд,танк,дроч,скотч,крейсер,дрочер,дорас", ",");
auto abusives             = split("блять,сука,ебать,пиздец,нахуй", ",");
auto dick_adjectives      = split("трахо,ебо,сексо,порно,конче,негро", ",");
auto dicks                = split("член,штырь,штепсель,кол,баклажан,трон,дик,ствол,крючок,питон,пайтон,шланг,кол", ",");
auto demonstrative_verbs  = split("иди,пиздуй,шуруй,вали,ебись", ",");
auto verbs                = split("утопил,отравил говном,переехал,на суп спустил,похоронил,из репозиториев установил,в деб пакет засунул,в рпм опакетил,скомпилировал,ебал,зарезал,продал за 3 рубля,отпиздил,с балкона выбросил,стирал,высушил,отравил газом,насиловал,трахал,пиздил,ножом резал,на хую вертел,на хуе до 12000 об/с разгонял,ссал на,срал на,продал,купил,проиграл в казино,выиграл в казино,выебал,обосрал,оплодотворял,продавал на металлолом,сдавал на чермет,продавал в секс-рабство,вчера в гроб ложил", ",");
auto relatives_impad      = split("мать,бабушку,сестру,бабку,мамку,тёщу,родню,пизду,ротовую полость,родину,деревню,сноху,дочь,дочку,одноклассницу,классную руководительницу,однокурсницу", ",");
auto places               = split("нахуй,с обрыва,под землю,маме отсасывать,сестре отлизывать,бабушке отлизывать,маме помогать,в пизду,отсюда,к хуям,по помойкам шарить,у бати сосать,маме отлизывать,маме жаловаться,сглатывать,фистить себе жопу,глотать сперму,сосать", ",");
auto whose                = split("мамкин,папкин,сосалкин,шалавкин,плюшкин", ",");
auto relatives            = split("мать,мамка,бабка,бабушка,дочка,тёща,сноха,жена", ",");
auto fem_insults          = split("шалава,лоханка,макака,обезьяна,шлюха,шаболда,сосалка,дура,молекула,хуйня,грязь,проститутка,пизда,махнатка,дырка,дыра,вонючка,конча,пылинка,хохлинка,корова,бабка,уродина,фиона,пепеляшка,акула,курица", ",");
auto fem_adjectives       = split("ебаная,обосраная,мёртвая,вообще жива?,обоссаная,поднадусёровая,слабонервная,жирная,вонючая,кастрированная,ебучая,невменяемая,блядская,черномазая,оттраханая,обдроченная,конченая", ",");
auto item_adjectives_fem  = split("широкая,огромная,мелкая,выебанная,гигантская,обдолбанна", ",");

std::string insult        = "Оскорбление не придумал";

std::string_view choice(const std::vector<std::string_view>& input) {
  std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<> distrib(0, input.size() - 1);
  return input[distrib(generator)];
}

std::string generate() {
  std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<> distrib(1, 6);
  switch (distrib(generator)) {
    case 1:
      return fmt::format("{} {}, {} {}", choice(demonstrative_verbs), choice(places), choice(insults), choice(adjectives));
    case 2:
      return fmt::format("{} ты {} {}", choice(abusives), choice(insults), choice(whose));
    case 3:
      return fmt::format("пососи мой {}-{}, {} {} {}", choice(dick_adjectives), choice(dicks), choice(adjectives), choice(whose), choice(whose));
    case 4:
      return fmt::format("{} ты {} {}", choice(abusives), choice(geninsult_first), choice(geninsult_endings));
    case 5:
      return fmt::format("да я твою {} {}, понимаешь?", choice(relatives_impad), choice(verbs));
    case 6:
      return fmt::format("{} {}, {} {}", choice(demonstrative_verbs), choice(places), choice(fem_insults), choice(fem_adjectives));
    default:
      exit(1);
      break;
  }
}

void run_affront_tests() {
  std::cout << generate() << std::endl;
  std::cout << generate() << std::endl;
  std::cout << generate() << std::endl;
  std::cout << generate() << std::endl;
  std::cout << generate() << std::endl;
  std::cout << generate() << std::endl;
  std::cout << generate() << std::endl;
  std::cout << generate() << std::endl;
  std::cout << generate() << std::endl;
  std::cout << generate() << std::endl;
  std::cout << generate() << std::endl;
}
