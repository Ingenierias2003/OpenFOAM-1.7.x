/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2010 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "motionDiffusivity.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(motionDiffusivity, 0);

    defineRunTimeSelectionTable(motionDiffusivity, Istream);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::motionDiffusivity::motionDiffusivity(const fvMotionSolver& mSolver)
:
    mSolver_(mSolver)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::motionDiffusivity> Foam::motionDiffusivity::New
(
    const fvMotionSolver& mSolver,
    Istream& mdData
)
{
    word diffTypeName(mdData);

    Info << "Selecting motion diffusion: " << diffTypeName << endl;

    IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(diffTypeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "motionDiffusivity::New(const tetPolyMesh& tetMesh, "
            "const Istream& dict)"
        )   << "Unknown diffusion type " << diffTypeName
            << endl << endl
            << "Valid diffusion types are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<motionDiffusivity>(cstrIter()(mSolver, mdData));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::motionDiffusivity::~motionDiffusivity()
{}


// ************************************************************************* //
