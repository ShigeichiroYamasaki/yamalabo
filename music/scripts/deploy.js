const hre = require("hardhat");

async function main() {
   const MusicToken = await hre.ethers.deployContract("MusicToken");
   await MusicToken.waitForDeployment();
}

main().catch((error) => {
   console.error(error);
   process.exitCode = 1;
});


async function main() {
  // コントラクトファクトリを取得
  const MusicToken = await ethers.getContractFactory("MusicToken");

  // 初期供給量を設定
  const initialSupply = 100;

  // コントラクトをデプロイ
  console.log("Deploying the MusicToken contract...");
  const musicToken = await MusicToken.deploy(initialSupply);
  await musicToken.deployed();

  console.log("MusicToken deployed to:", musicToken.address);
}

main()
  .then(() => process.exit(0))
  .catch((error) => {
    console.error(error);
    process.exit(1);
  });