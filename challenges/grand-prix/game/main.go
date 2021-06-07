package main

import (
	"fmt"
	"sync"
	"time"
	"math"
)

type vehicle interface {
	accelerate()
	decelerate()
	rebase()
}

type Car struct {
	Driver          string `json:"driver"`
	Team            string `json:"team"`
	Lane		int
	GearSpeeds     []int  `json:"gearSpeeds"`
	CurrentGear    int    `json:"currentGear"`
	CurrentDistance int    `json:"currentDistance"`
	Revs int
}

func (car *Car) accelerate() {

	// If there is a turn, decelerate
	for _, turn := range(race.turns) {
		distanceToTurn := turn - car.CurrentDistance%race.lapDistance
		if distanceToTurn >= 0 && (distanceToTurn < 50 || distanceToTurn <= car.GearSpeeds[car.CurrentGear]) {
			car.decelerate(nil)
		}
	}

	// If there is a car in front of same lane and acceleration causes colliding, then rebase
	for _, otherCar := range(cars) {
		if otherCar.Driver != car.Driver {
			if otherCar.Lane == car.Lane {
				distance := otherCar.CurrentDistance - car.CurrentDistance
				speedDiff := car.GearSpeeds[car.CurrentGear] - otherCar.GearSpeeds[otherCar.CurrentGear]
				if distance <= speedDiff && distance >= 0 && speedDiff > 0 {
					fmt.Println(car.Driver, "attempts to rebase ", otherCar.Driver)
					success := car.rebase()
					if success == false {
						car.decelerate(&otherCar)
					}
				}
			}
		}
	}

	// Car shifts to next gear if it exists
	if car.CurrentGear < len(car.GearSpeeds)-1 {
		if car.CurrentGear == 3 && car.Revs%2 == 0 { 
			car.CurrentGear++
		} else if car.CurrentGear >= 4 && car.Revs % 7 == 0 {
			car.CurrentGear++
		} else if car.CurrentGear <= 2 {
			car.CurrentGear++
		}
		car.Revs++
	}

	// Now move
	car.CurrentDistance += car.GearSpeeds[car.CurrentGear]
	wgDrivers.Done()
}

func (car *Car) decelerate(carInFront *Car) {

	fmt.Println("Seems that ", car.Driver, "is decelerating!")
	// Go to second gear in case of turn
	if carInFront == nil {
		car.CurrentGear = 1
		return
	}

	// Choose best gear based on speed of car in front, it should be <= than that.
	frontSpeed := carInFront.GearSpeeds[carInFront.CurrentGear]
	bestGear := 0
	for i, speed := range(car.GearSpeeds) {
		if speed <= frontSpeed {
			bestGear = i
		} else {
			break
		}
	}

	car.CurrentGear = bestGear
}

func (car *Car) rebase() bool {

	newLane := car.Lane
	// Car should look at both sides to see if there's an open lane to move laterally.
	for _, otherCar := range(cars) {
		if otherCar.Driver != car.Driver {
			if otherCar.Lane == car.Lane + 1 {
				distance := otherCar.CurrentDistance - car.CurrentDistance
				if math.Abs(float64(distance)) > 50 {
					newLane = car.Lane + 1
				}
			} else if otherCar.Lane == car.Lane - 1 {
				distance := otherCar.CurrentDistance - car.CurrentDistance
				if math.Abs(float64(distance)) > 50 {
					newLane = car.Lane - 1
				}
			}
		}
	}

	// Switch lane
	if car.Lane != newLane {
		fmt.Println("SUCCESSFUL REBASE by", car.Driver, car.Lane, newLane)
		car.Lane = newLane
		return true
	} else {
		return false
	}

}

type Race struct {
	name string
	laps int
	turns []int
	lapDistance int
	finished bool
	time int
}

func (race *Race) print() {
	for _, car := range(cars) {
		for i := 0; i< car.CurrentDistance/100; i++{
			fmt.Printf("#")
		}
		fmt.Printf("-%s (%d, %d, %d)\n", car.Driver, car.Lane, car.CurrentDistance, car.CurrentGear)
	}
	fmt.Printf("\n\n\n\n\n")

}

var race = Race {
	name: "Mexico GP",
	laps: 30,
	turns: []int{230, 2090, 5500, 5900, 6199, 7680},
	lapDistance: 2300,
	finished: false,
	time: 0,
}

var cars = []Car{
	Car {
		Driver: "J. Chávez",
		Team: "Ferrari",
		GearSpeeds: []int{0, 50, 65, 70, 90, 120},
		Lane: 0,
		CurrentGear: 0,
		CurrentDistance: 1000,
	},
	Car {
		Driver: "C. Pérez",
		Team: "Red Bull",
		GearSpeeds: []int{0, 30, 60, 70, 85, 99, 108},
		Lane: 0,
		CurrentGear: 0,
		CurrentDistance: 100,
	},
	Car {
		Driver: "M. Verstappen",
		Team: "Red Bull",
		GearSpeeds: []int{0, 40, 60, 75, 80, 110, 115},
		Lane: 1,
		CurrentGear: 0,
		CurrentDistance: 0,
	},
	Car {
		Driver: "L. Hamilton",
		Team: "Red Bull",
		GearSpeeds: []int{0, 30, 50, 75, 83, 100, 122},
		Lane: 1,
		CurrentGear: 0,
		CurrentDistance: 100,
	},
	Car {
		Driver: "E. Musk",
		Team: "Tesla",
		GearSpeeds: []int{0, 40, 70, 95, 120, 120, 122},
		Lane: 1,
		CurrentGear: 0,
		CurrentDistance: 200,
	},
	Car {
		Driver: "Juan",
		Team: "America",
		GearSpeeds: []int{0, 10, 30, 45, 90, 135, 160},
		Lane: 0,
		CurrentGear: 0,
		CurrentDistance: 300,
	},
	Car {
		Driver: "Marcial Maciel",
		Team: "Tesla",
		GearSpeeds: []int{0, 70, 98, 110, 112, 113},
		Lane: 1,
		CurrentGear: 0,
		CurrentDistance: 400,
	},
}

var wgDrivers sync.WaitGroup

func main() {
	fmt.Println(race.name)
	race.print()


	for step := 0; step < 100; step++ {

		wgDrivers.Add(len(cars))
		cars[0].accelerate()
		cars[1].accelerate()
		cars[2].accelerate()
		cars[3].accelerate()
		cars[4].accelerate()
		cars[5].accelerate()
		cars[6].accelerate()

		wgDrivers.Wait()
		race.print()
		time.Sleep(75 * time.Millisecond)
	}

}
