#include "reservation.h"
#include "communication-protocol.h"

using json = nlohmann::json;

int main() {
    auto r1 = Reservation::create("12.12.2015 00:00:15", 1, "User1"),
         r2 = Reservation::create("6.12.2015 00:10:15", 2, "User2");
    std::vector<Reservation> reservations{r1, r2};
    auto calendar = CommunicationProtocol::createCalendarFor(reservations);
    std::cout << json::parse(calendar).dump(4) << std::endl;
    auto reservationsObtained = CommunicationProtocol::getCalendar(calendar);
    for(auto& res: reservations)
        std::cout << res.startToString() << " " << res.duration() << std::endl;

    auto singleReservation = CommunicationProtocol::createReservationFor(r1);
    auto obtainedSingleReservation = CommunicationProtocol::getReservation(singleReservation);
    std::cout << obtainedSingleReservation.startToString() << " "
              << obtainedSingleReservation.duration() << std::endl;
}